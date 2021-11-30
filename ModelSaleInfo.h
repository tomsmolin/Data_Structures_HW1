#ifndef MODEL_SALE_INFO_H
#define MODEL_SALE_INFO_H

class ModelSaleInfo
{
    public:
        int sales_num, type_id, model_id;
        ModelSaleInfo (int sales_num, int type_id, int model_id) : sales_num(sales_num), type_id(type_id),
                                                                    model_id(model_id) {}
        ~ModelSaleInfo() = default;

    // ======= Operators =======
    bool operator< (const ModelSaleInfo& model_info_two);
    bool operator== (const ModelSaleInfo& model_info_two);
};

#endif //MODEL_SALE_INFO_H