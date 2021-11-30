#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#define NOT_INIT 0

class TypeInfo
{ 
    public:
        int* grades_models;
        int* sales_models;

        int models_num;
        int best_seller_model;
        int best_seller_sales;

        TypeInfo (int models_num) : grades_models(nullptr), sales_models(nullptr),
                                    models_num(models_num),best_seller_model(NOT_INIT), best_seller_sales(NOT_INIT) {}
        ~TypeInfo() = default;
};

#endif //TYPE_INFO_H