// file:  processNxCoreCategoryMessage.cpp

#include <iomanip>
#include "processNxCoreCategoryMessage.hpp"

extern NxCoreClass NxCore; // defined in main.cpp

// processNxCoreCategoryMessage: Function to handle NxCore Category messages.
// In this example we only capture category 4 and category 13 messages.
//--------------------------------------------------------------

int processNxCoreCategoryMessage(
    [[maybe_unused]] const NxCoreSystem *pNxCoreSys,
    const NxCoreMessage *pNxCoreMsg)
{
    // Category field pointer
    NxCategoryField *pField;

    // Do something based on the category num
    switch (pNxCoreMsg->coreData.Category.pnxStringCategory->Atom)
    {
    // Category 4, Equity Symbol Information
    case 4:
        // Grab a pointer to the field
        pField = &pNxCoreMsg->coreData.Category.pnxFields[0];
        // Is field set?
        if (pField->Set == 1)
        {
            // Print the Company Name
            cout << "Category 4 for Symbol: " << pNxCoreMsg->coreHeader.pnxStringSymbol->String << "," << " Company Name= " << pField->data.StringZ << endl;
        }
        break;

    // Category 13, 52 Week High/Low
    case 13:
        double High52 = 0.0, Low52 = 0.0;
        NxDate HighDate;
        NxDate LowDate;
        memset(&HighDate, 0, sizeof(NxDate));
        memset(&LowDate, 0, sizeof(NxDate));
        // Grab a pointer to the 52 week high date
        pField = &pNxCoreMsg->coreData.Category.pnxFields[0];
        if (pField->Set == 1)
            memcpy(&HighDate, &pField->data.nxDate, sizeof(NxDate));
        // Grab a pointer to the 52 week low date
        pField = &pNxCoreMsg->coreData.Category.pnxFields[1];
        if (pField->Set == 1)
            memcpy(&LowDate, &pField->data.nxDate, sizeof(NxDate));
        // Grab a pointer to the 52 week high field
        pField = &pNxCoreMsg->coreData.Category.pnxFields[2];
        // Get the 52 week high
        if (pField->Set == 1)
            High52 = NxCore.PriceToDouble(pField->data.nxPrice.Price, pField->data.nxPrice.PriceType);
        // Grab a pointer to the 52 week low field
        pField = &pNxCoreMsg->coreData.Category.pnxFields[3];
        // Get the 52 week low
        if (pField->Set == 1)
            Low52 = NxCore.PriceToDouble(pField->data.nxPrice.Price, pField->data.nxPrice.PriceType);
        cout.fill('0');
        cout << "Category 13 for Symbol: " << pNxCoreMsg->coreHeader.pnxStringSymbol->String
             << ", " << setw(2) << static_cast<int>(HighDate.Month)
             << "/" << setw(2) << static_cast<int>(HighDate.Day)
             << "/" << setw(4) << static_cast<int>(HighDate.Year)
             << " High52= " << fixed << setprecision(2) << High52
             << ", " << setw(2) << static_cast<int>(LowDate.Month)
             << "/" << setw(2) << static_cast<int>(LowDate.Day)
             << "/" << setw(4) << static_cast<int>(LowDate.Year)
             << " Low52= " << fixed << setprecision(2) << Low52 << endl;
        break;
    }
    return NxCALLBACKRETURN_CONTINUE;
}
