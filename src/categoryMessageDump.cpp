// file: categoryMessageDump.cpp

// Thanks for looking throught the code!
// This is a little bonus file ported over for you to check out.

#include <cstdio>
#include <string>
#include <string_view>
#include <fmt/format.h> // assuming you have {fmt} installed/available

#include "NxCoreAPI.h"
#include "NxCoreAPI_Wrapper_C++.h"

extern NxCoreClass NxCore; // In production: consider singleton / dependency injection

namespace
{

    std::string makeOptionSymbol(const NxCoreHeader &hdr)
    {
        if (!hdr.pnxOptionHdr)
        {
            return hdr.pnxStringSymbol ? hdr.pnxStringSymbol->String : "(null)";
        }

        const auto &opt = *hdr.pnxOptionHdr;
        const auto &base = *hdr.pnxStringSymbol;

        // OSI format (most common nowadays)
        if (opt.pnxsDateAndStrike && opt.pnxsDateAndStrike->String[1] == ' ')
        {
            return fmt::format("{}{:02d}{:02d}{:02d}{}{:08d}",
                               base.String,
                               opt.nxExpirationDate.Year % 100, // usually 00-99
                               opt.nxExpirationDate.Month,
                               opt.nxExpirationDate.Day,
                               (opt.PutCall == 0) ? 'C' : 'P',
                               opt.strikePrice);
        }

        // Legacy OPRA / short format
        if (opt.pnxsDateAndStrike)
        {
            return fmt::format("{}{}{}",
                               base.String,
                               opt.pnxsDateAndStrike->String[0],
                               opt.pnxsDateAndStrike->String[1]);
        }

        // fallback
        return base.String ? base.String : "(no symbol)";
    }

    std::string_view getPermissionName(int perm_id)
    {
        if (const char *name = NxCore.GetDefinedString(NxST_PERMID, perm_id))
        {
            return std::string_view{name};
        }
        return "???";
    }

    void print_category_field(const NxCategoryField &cf, int index, std::FILE *out = stdout)
    {
        std::fprintf(out, "     %2d | %-12s | %-20s | Set:%d : ",
                     index, cf.FieldName ? cf.FieldName : "(null)",
                     cf.FieldInfo ? cf.FieldInfo : "(no info)", cf.Set);

        if (!cf.Set)
        {
            std::fputs(" -- not set --\n", out);
            return;
        }

        switch (cf.FieldType)
        {
        case NxCFT_UNKNOWN:
            std::fputs(" unknown type\n", out);
            break;

        case NxCFT_64BIT:
            std::fprintf(out, "%lld\n", cf.data.i64Bit);
            break;

        case NxCFT_32BIT:
            std::fprintf(out, "%d\n", cf.data.i32Bit);
            break;

        case NxCFT_STRINGZ:
            std::fprintf(out, "\"%s\"\n", cf.data.StringZ ? cf.data.StringZ : "(null)");
            break;

        case NxCFT_DOUBLE:
            std::fprintf(out, "%.6g\n", cf.data.Double);
            break;

        case NxCFT_PRICE:
        {
            double price = NxCore.PriceToDouble(cf.data.nxPrice.Price, cf.data.nxPrice.PriceType);
            std::fprintf(out, "%.2f\n", price);
            break;
        }

        case NxCFT_DATE:
            std::fprintf(out, "%04d-%02d-%02d\n",
                         cf.data.nxDate.Year, cf.data.nxDate.Month, cf.data.nxDate.Day);
            break;

        case NxCFT_TIME:
            std::fprintf(out, "%02d:%02d:%02d.%03d\n",
                         cf.data.nxTime.Hour, cf.data.nxTime.Minute,
                         cf.data.nxTime.Second, cf.data.nxTime.Millisecond);
            break;

        case NxCFT_NxSTRING:
            if (cf.data.pnxString && cf.data.pnxString->String)
            {
                std::fprintf(out, "\"%s\"\n", cf.data.pnxString->String);
            }
            else
            {
                std::fputs("(null NxString)\n", out);
            }
            break;

        case NxCFT_STRING_IDX:
        {
            const auto &st = cf.data.stringTableItem;
            const char *txt = NxCore.GetDefinedString(static_cast<int>(st.ixTable),
                                                      static_cast<int>(st.idString));
            std::fprintf(out, "[%d:%u] \"%s\"\n",
                         st.ixTable, st.idString, txt ? txt : "***NULL***");
            break;
        }

        case NxCFT_STRING_MAP:
        {
            const auto &st = cf.data.stringTableItem;
            unsigned int bits = st.idString;
            bool first = true;

            std::fputs(" flags: ", out);

            for (int i = 0; bits != 0; ++i, bits >>= 1)
            {
                if (bits & 1)
                {
                    const char *txt = NxCore.GetDefinedString(static_cast<int>(st.ixTable), i);
                    if (!first)
                        std::fputs(", ", out);
                    std::fprintf(out, "[%d:%d] \"%s\"", st.ixTable, i, txt ? txt : "***NULL***");
                    first = false;
                }
            }
            std::fputc('\n', out);
            break;
        }

        default:
            std::fprintf(out, "<unknown type %d>\n", cf.FieldType);
            break;
        }
    }

} // anonymous namespace

void categoryMessageDump(const NxCoreSystem *sys, const NxCoreMessage *msg)
{
    if (!sys || !msg)
    {
        std::fputs("categoryMessageDump: invalid pointer(s)\n", stderr);
        return;
    }

    const auto &hdr = msg->coreHeader;
    const auto &cat = msg->coreData.Category;

    const std::string symbol = makeOptionSymbol(hdr);

    std::puts("═══════════════════════════════════════════════");
    std::puts("Category Message Dump:");
    std::printf("  Category       : %s (%d)\n",
                cat.pnxStringCategory && cat.pnxStringCategory->String
                    ? cat.pnxStringCategory->String
                    : "(null)",
                cat.pnxStringCategory ? cat.pnxStringCategory->Atom : -1);

    if (sys->nxTime.Hour || sys->nxTime.Minute || sys->nxTime.Second)
    {
        std::printf("  NxCore Time    : %02d:%02d:%02d\n",
                    sys->nxTime.Hour, sys->nxTime.Minute, sys->nxTime.Second);
    }

    std::printf("  Symbol         : %s\n", symbol.c_str());

    std::printf("  Listed Exch    : %d\n", hdr.ListedExg);
    std::printf("  Session Date   : %04d-%02d-%02d\n",
                hdr.nxSessionDate.Year, hdr.nxSessionDate.Month, hdr.nxSessionDate.Day);
    std::printf("  Exg Timestamp  : %02d:%02d:%02d\n",
                hdr.nxExgTimestamp.Hour, hdr.nxExgTimestamp.Minute, hdr.nxExgTimestamp.Second);

    std::printf("  Reporting Exch : %d\n", hdr.ReportingExg);
    std::printf("  Session ID     : %d\n", hdr.SessionID);

    std::string_view perm_name = getPermissionName(hdr.PermissionID);
    std::printf("  Permission     : %d [%.*s]\n",
                hdr.PermissionID,
                static_cast<int>(perm_name.size()),
                perm_name.data());

    std::printf("  Field Count    : %d\n", cat.NFields);

    if (cat.NFields > 0 && cat.pnxFields)
    {
        std::puts("  Fields:");
        for (int i = 0; i < cat.NFields; ++i)
        {
            print_category_field(cat.pnxFields[i], i);
        }
    }
    else
    {
        std::puts("  No fields present");
    }

    std::puts("═══════════════════════════════════════════════");
}