// file: categoryMessageDump.cpp

// Thanks for looking throught the code!
// This is a little bonus file ported over for you to check out.

#include <cstdio>
#include <fmt/format.h>
#include <string>
#include <string_view>

#include "NxCoreAPI.h"
#include "NxCoreAPI_Wrapper_C++.h"

extern NxCoreClass NxCore; // In production, make this a singleton or pass as parameter

namespace
{

    std::string makeOptionSymbol(const NxCoreHeader &hdr)
    {
        if (!hdr.pnxOptionHdr)
        {
            return std::string(hdr.pnxStringSymbol->String);
        }

        const auto &opt = *hdr.pnxOptionHdr;
        const auto &base = *hdr.pnxStringSymbol;

        if (opt.pnxsDateAndStrike->String[1] == ' ')
        {
            // OSI format
            return fmt::format("{}{:02d}{:02d}{:02d}{}{:08d}",
                               base.String,
                               opt.nxExpirationDate.Year - 2000,
                               opt.nxExpirationDate.Month,
                               opt.nxExpirationDate.Day,
                               (opt.PutCall == 0) ? 'C' : 'P',
                               opt.strikePrice);
        }
        else
        {
            // Legacy OPRA format
            return fmt::format("{}{}{}",
                               base.String,
                               opt.pnxsDateAndStrike->String[0],
                               opt.pnxsDateAndStrike->String[1]);
        }
    }

    std::string_view getPermissionName(int perm_id)
    {
        const char *name = NxCore.GetDefinedString(NxST_PERMID, perm_id);
        return name ? std::string_view{name} : std::string_view{"???"};
    }

    void print_category_field(const NxCategoryField &cf, int index)
    {
        std::printf("     %d|%s|[%s] {%d} :",
                    index, cf.FieldName, cf.FieldInfo, cf.Set);

        if (!cf.Set)
        {
            std::puts("  -- !Set --");
            return;
        }

        switch (cf.FieldType)
        {
        case NxCFT_UNKNOWN:
            std::puts("  -- unknown --");
            break;

        case NxCFT_64BIT:
            std::printf("  %lld\n", cf.data.i64Bit);
            break;

        case NxCFT_32BIT:
            std::printf("  %d\n", cf.data.i32Bit);
            break;

        case NxCFT_STRINGZ:
            std::printf("  %s\n", cf.data.StringZ);
            break;

        case NxCFT_DOUBLE:
            std::printf("  %.6g\n", cf.data.Double);
            break;

        case NxCFT_PRICE:
        {
            double price = NxCore.PriceToDouble(cf.data.nxPrice.Price, cf.data.nxPrice.PriceType);
            std::printf("  %.2f\n", price);
            break;
        }

        case NxCFT_DATE:
            std::printf("  %04d%02d%02d\n",
                        cf.data.nxDate.Year,
                        cf.data.nxDate.Month,
                        cf.data.nxDate.Day);
            break;

        case NxCFT_TIME:
            std::printf("  %02d:%02d:%02d.%03d\n",
                        cf.data.nxTime.Hour,
                        cf.data.nxTime.Minute,
                        cf.data.nxTime.Second,
                        cf.data.nxTime.Millisecond);
            break;

        case NxCFT_NxSTRING:
            if (cf.data.pnxString)
            {
                std::printf("  '%s'\n", cf.data.pnxString->String);
            }
            else
            {
                std::puts("  (null)");
            }
            break;

        case NxCFT_STRING_IDX:
        {
            const auto &st = cf.data.stringTableItem;
            const char *txt = NxCore.GetDefinedString(
                static_cast<int>(st.ixTable),
                static_cast<int>(st.idString));
            std::printf("  [%d:%u] '%s'\n",
                        st.ixTable, st.idString,
                        txt ? txt : "***NULL***");
            break;
        }

        case NxCFT_STRING_MAP:
        {
            const auto &st = cf.data.stringTableItem;
            unsigned int bits = st.idString;
            bool first = true;

            for (int i = 0; bits != 0; ++i, bits >>= 1)
            {
                if (bits & 1)
                {
                    const char *txt = NxCore.GetDefinedString(
                        static_cast<int>(st.ixTable),
                        static_cast<int>(i));
                    if (!first)
                    {
                        std::printf(", ");
                    }
                    std::printf("[%d:%d] '%s'", st.ixTable, i, txt ? txt : "***NULL***");
                    first = false;
                }
            }
            std::putchar('\n');
            break;
        }

        default:
            std::printf("  <unknown type %d>\n", cf.FieldType);
            break;
        }
    }

} // namespace

void categoryMessageDump(const NxCoreSystem *sys, const NxCoreMessage *msg)
{
    if (!sys || !msg)
    {
        std::fputs("categoryDump: invalid pointer(s)\n", stderr);
        return;
    }

    const auto &hdr = msg->coreHeader;
    const auto &cat = msg->coreData.Category;

    auto symbol = makeOptionSymbol(hdr);

    std::puts("Category dump:");
    std::printf("  Category       : %s (%d)\n",
                cat.pnxStringCategory->String,
                cat.pnxStringCategory->Atom);

    std::printf("  nxTime         : %02d:%02d:%02d\n",
                sys->nxTime.Hour, sys->nxTime.Minute, sys->nxTime.Second);

    std::printf("  symbol         : %s\n", symbol.c_str());

    std::printf("  listedExch     : %d\n", hdr.ListedExg);
    std::printf("  sessionDate    : %04d%02d%02d\n",
                hdr.nxSessionDate.Year, hdr.nxSessionDate.Month, hdr.nxSessionDate.Day);
    std::printf("  time           : %02d:%02d:%02d\n",
                hdr.nxExgTimestamp.Hour, hdr.nxExgTimestamp.Minute, hdr.nxExgTimestamp.Second);

    std::printf("  reportExch     : %d\n", hdr.ReportingExg);
    std::printf("  sessionId      : %d\n", hdr.SessionID);

    std::printf("  permissionId   : %d [%.*s]\n",
                hdr.PermissionID,
                static_cast<int>(getPermissionName(hdr.PermissionID).size()),
                getPermissionName(hdr.PermissionID).data());

    std::printf("  NFields        : %d\n", cat.NFields);

    if (cat.NFields > 0 && cat.pnxFields)
    {
        for (int i = 0; i < cat.NFields; ++i)
        {
            print_category_field(cat.pnxFields[i], i);
        }
    }
}