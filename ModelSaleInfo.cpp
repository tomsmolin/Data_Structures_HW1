#include "ModelSaleInfo.h"

bool ModelSaleInfo::operator<(const ModelSaleInfo& model_info_two)
{
    if(sales_num < model_info_two.sales_num)
    {
        return true;
    }

    if (sales_num > model_info_two.sales_num)
    {
        return false;
    }

    if(type_id > model_info_two.type_id)
    {
        return true;
    }

    if (type_id < model_info_two.type_id)
    {
        return false;
    }
    
    return model_id > model_info_two.model_id;
}

bool ModelSaleInfo::operator== (const ModelSaleInfo& model_info_two)
{
    if (sales_num == model_info_two.sales_num && type_id == model_info_two.type_id &&
        model_id == model_info_two.model_id)
    {
        return true;
    }
    return false;
}