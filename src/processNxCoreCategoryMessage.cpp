// processNxCoreCategoryMessage.cpp

// processNxCoreCategoryMessage: Function to handle NxCore Category messages.
// In this example we only capture category 4 and category 13 messages.
//--------------------------------------------------------------

#include "processNxCoreCategoryMessage.hpp"

#include <iomanip>
#include <iostream>
#include <string_view>

extern NxCoreClass NxCore;

namespace
{

    constexpr int CATEGORY_EQUITY_SYMBOL_INFO = 4;
    constexpr int CATEGORY_52WEEK_HIGH_LOW = 13;

    // Safe for normal NxString* used in category/symbol messages (null-terminated)
    constexpr std::string_view to_string_view(const NxString *nxstr) noexcept
    {
        // nxstr->String is never null if nxstr != nullptr (flexible array member)
        return nxstr ? std::string_view{nxstr->String} : std::string_view{};
    }

    // For NxCategoryField .data.StringZ — assuming null-terminated when Set==1
    constexpr std::string_view to_string_view(const NxCategoryField &field) noexcept
    {
        return (field.Set == 1 && field.data.StringZ)
                   ? std::string_view{field.data.StringZ}
                   : std::string_view{};
    }

    struct DateYMD
    {
        int year = 0;
        int month = 0;
        int day = 0;

        explicit DateYMD(const NxDate &d) noexcept
            : year(d.Year), month(d.Month), day(d.Day)
        {
        }

        explicit DateYMD() = default;
    };

    std::ostream &operator<<(std::ostream &os, const DateYMD &date)
    {
        os << std::setfill('0')
           << std::setw(2) << date.month << '/'
           << std::setw(2) << date.day << '/'
           << std::setw(4) << date.year;
        return os;
    }

} // namespace

[[nodiscard]] int processNxCoreCategoryMessage(
    const NxCoreSystem * /*pNxCoreSys*/,
    const NxCoreMessage *pNxCoreMsg,
    std::ostream &out) noexcept
{
    if (!pNxCoreMsg || !pNxCoreMsg->coreData.Category.pnxStringCategory)
    {
        return NxCALLBACKRETURN_CONTINUE;
    }

    const auto category = pNxCoreMsg->coreData.Category.pnxStringCategory->Atom;

    const auto symbol = to_string_view(pNxCoreMsg->coreHeader.pnxStringSymbol);

    switch (category)
    {
    case CATEGORY_EQUITY_SYMBOL_INFO:
    {
        const auto &field = pNxCoreMsg->coreData.Category.pnxFields[0];

        if (field.Set == 1)
        {
            const auto company_name = to_string_view(field);

            out << "Category 4 for Symbol: " << symbol
                << ", Company Name = " << company_name << '\n';
        }
        break;
    }

    case CATEGORY_52WEEK_HIGH_LOW:
    {
        const auto &fields = pNxCoreMsg->coreData.Category.pnxFields;

        // Field layout assumption (based on original code):
        //   [0] = 52-week high date
        //   [1] = 52-week low date
        //   [2] = 52-week high price
        //   [3] = 52-week low price

        DateYMD high_date;
        DateYMD low_date;
        double high_52w = 0.0;
        double low_52w = 0.0;

        if (fields[0].Set == 1)
            high_date = DateYMD{fields[0].data.nxDate};

        if (fields[1].Set == 1)
            low_date = DateYMD{fields[1].data.nxDate};

        if (fields[2].Set == 1)
            high_52w = NxCore.PriceToDouble(
                fields[2].data.nxPrice.Price,
                fields[2].data.nxPrice.PriceType);

        if (fields[3].Set == 1)
            low_52w = NxCore.PriceToDouble(
                fields[3].data.nxPrice.Price,
                fields[3].data.nxPrice.PriceType);

        out << "Category 13 for Symbol: " << symbol << ", "
            << high_date << " High52= " << std::fixed << std::setprecision(2) << high_52w
            << ", " << low_date << " Low52= " << std::fixed << std::setprecision(2) << low_52w
            << '\n';

        break;
    }

    default:
        // We only handle categories 4 and 13 → do nothing for others
        break;
    }

    return NxCALLBACKRETURN_CONTINUE;
}