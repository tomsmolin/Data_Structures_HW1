#include "ModelGradeInfo.h"

bool ModelGradeInfo::operator<(const ModelGradeInfo& model_info_two)
{
    if(grade < model_info_two.grade)
    {
        return true;
    }

    if (grade > model_info_two.grade)
    {
        return false;
    }

    if(type_id < model_info_two.type_id)
    {
        return true;
    }

    if (type_id > model_info_two.type_id)
    {
        return false;
    }
    
    return model_id < model_info_two.model_id;
}

bool ModelGradeInfo::operator== (const ModelGradeInfo& model_info_two)
{
    if (grade == model_info_two.grade && type_id == model_info_two.type_id &&
        model_id == model_info_two.model_id)
    {
        return true;
    }
    return false;
}