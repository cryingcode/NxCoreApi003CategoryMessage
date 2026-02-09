// processNxCoreSymbolSpinMessage.cpp

#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <cstring> // strlen

#include "processNxCoreSymbolSpinMessage.hpp"

extern NxCoreClass NxCore;

[[nodiscard]] int processNxCoreSymbolSpinMessage(
    const NxCoreSystem * /*pNxCoreSys*/, // Unused for now, but included for future context needs
    const NxCoreMessage *pNxCoreMsg,
    std::ostream &out) noexcept
{
    if (!pNxCoreMsg || !pNxCoreMsg->coreHeader.pnxStringSymbol)
    {
        return NxCALLBACKRETURN_CONTINUE;
    }

    const auto *sym = pNxCoreMsg->coreHeader.pnxStringSymbol;
    const auto *optHdr = pNxCoreMsg->coreHeader.pnxOptionHdr;

    if (sym->String[0] != 'o' || !optHdr)
    {
        return NxCALLBACKRETURN_CONTINUE;
    }

    const std::string_view nx_symbol(sym->String); // null-terminated

    const double strike_price = optHdr->strikePrice / 1000.0;

    std::string underlying;
    std::string series;

    if (optHdr->pnxsUnderlying && optHdr->pnxsUnderlying->String[0] != '\0')
    {
        underlying = optHdr->pnxsUnderlying->String;
    }

    if (optHdr->pnxsSeriesChain && optHdr->pnxsSeriesChain->String[0] != '\0')
    {
        series = optHdr->pnxsSeriesChain->String;
    }

    std::string readable_symbol;

    const auto *date_strike = optHdr->pnxsDateAndStrike;

    if (date_strike && date_strike->String[0]) // Silences warning, same safety check is done in NxCore server before sending the message with a non-null pnxsDateAndStrike pointer.
    {
        // OSI format check (exactly 2 chars, not null-terminated)
        if (date_strike->String[1] == ' ')
        {
            const std::string_view root(nx_symbol.data() + 1);

            const int yy = optHdr->nxExpirationDate.Year % 100;

            std::ostringstream oss;
            oss << std::left << std::setw(6) << root
                << std::setfill('0')
                << std::setw(2) << yy
                << std::setw(2) << optHdr->nxExpirationDate.Month
                << std::setw(2) << optHdr->nxExpirationDate.Day
                << ((optHdr->PutCall == 0) ? 'C' : 'P')
                << std::setfill('0') << std::setw(8) << optHdr->strikePrice;

            readable_symbol = oss.str();

            out << "Symbol Spin for: "
                << " NxSym: " << nx_symbol
                << " OSI Sym: " << readable_symbol
                << " Strike: " << std::fixed << std::setprecision(2) << strike_price
                << " Underlying: " << underlying
                << " Series Chain: " << series
                << '\n';
        }
        // Legacy OPRA
        else
        {
            std::ostringstream oss;
            oss << (nx_symbol.data() + 1)
                << date_strike->String[0]
                << date_strike->String[1];

            readable_symbol = oss.str();

            out << "Symbol Spin for: "
                << " NxSym: " << nx_symbol
                << " OPRA Sym: " << readable_symbol
                << " Strike: " << std::fixed << std::setprecision(2) << strike_price
                << " Underlying: " << underlying
                << " Series Chain: " << series
                << '\n';
        }
    }
    else
    {
        out << "Symbol Spin (no date/strike): NxSym: " << nx_symbol
            << " Strike: " << std::fixed << std::setprecision(2) << strike_price
            << '\n';
    }

    return NxCALLBACKRETURN_CONTINUE;
}