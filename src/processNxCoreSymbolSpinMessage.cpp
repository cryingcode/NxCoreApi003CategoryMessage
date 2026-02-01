// file:  processNxCoreSymbolSpinMessage.cpp

#include <iomanip>
#include "processNxCoreSymbolSpinMessage.hpp"

extern NxCoreClass NxCore; // defined in main.cpp

int processNxCoreSymbolSpinMessage(
    [[maybe_unused]] const NxCoreSystem *pNxCoreSys,
    const NxCoreMessage *pNxCoreMsg)
{
    // If this is an option, and there is a valid option header, use it
    if ((pNxCoreMsg->coreHeader.pnxStringSymbol->String[0] == 'o') && (pNxCoreMsg->coreHeader.pnxOptionHdr != NULL))
    {
        // Get the strike price from header info
        double StrikePrice;
        StrikePrice = pNxCoreMsg->coreHeader.pnxOptionHdr->strikePrice / 1000.0;

        char OptionSymbol[40];
        char UnderlyingStr[25];
        char SeriesStr[255];
        UnderlyingStr[0] = SeriesStr[0] = OptionSymbol[0] = 0;

        // Get the DateStrike string, Series string and Underlying symbol string
        // NOTE: VERY IMPORTANT TO NOT PASS NULL POINTERS IN AS ADDY FOR A STRING!!
        // SOME OF THESE STRINGS MAY OCCASIONALLY BE NULL
        if (pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsUnderlying)
            strcpy(UnderlyingStr, pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsUnderlying->String);
        if (pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsSeriesChain)
            strcpy(SeriesStr, pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsSeriesChain->String);

        // If pnxsDateAndStrike->String[1] == ' ', then this symbol is in new OSI format.
        if (pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsDateAndStrike->String[1] == ' ')
        {
            // Construct OSI Symbol, take out 'o' from symbol root (String+1)

            sprintf(OptionSymbol, "%-6s%02d%02d%02d%c%08d",
                    pNxCoreMsg->coreHeader.pnxStringSymbol->String + 1,
                    pNxCoreMsg->coreHeader.pnxOptionHdr->nxExpirationDate.Year - 2000,
                    pNxCoreMsg->coreHeader.pnxOptionHdr->nxExpirationDate.Month,
                    pNxCoreMsg->coreHeader.pnxOptionHdr->nxExpirationDate.Day,
                    (pNxCoreMsg->coreHeader.pnxOptionHdr->PutCall == 0) ? 'C' : 'P',
                    pNxCoreMsg->coreHeader.pnxOptionHdr->strikePrice);

            // Print the option symbol, underlying symbol and the series chain
            cout << "Symbol Spin for: " << " NxSym: " << pNxCoreMsg->coreHeader.pnxStringSymbol->String << " OSI Sym: " << OptionSymbol << " Strike: " << fixed << setprecision(2) << StrikePrice << " Underlying: " << UnderlyingStr << " Series Chain: " << SeriesStr << endl;
        }

        // Otherwise the symbol is in old OPRA format.
        // Take out 'o' from symbol series (String+1)
        else
        {
            sprintf(OptionSymbol, "%s%c%c",
                    pNxCoreMsg->coreHeader.pnxStringSymbol->String + 1,
                    pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsDateAndStrike->String[0],
                    pNxCoreMsg->coreHeader.pnxOptionHdr->pnxsDateAndStrike->String[1]);

            // Print the option symbol, underlying symbol and the series chain
            cout << "Symbol Spin for: " << " NxSym: " << pNxCoreMsg->coreHeader.pnxStringSymbol->String << " OPRA Sym: " << OptionSymbol << " Strike: " << fixed << setprecision(2) << StrikePrice << " Underlying: " << UnderlyingStr << " Series Chain: " << SeriesStr << endl;
        }
    }
    return NxCALLBACKRETURN_CONTINUE;
}