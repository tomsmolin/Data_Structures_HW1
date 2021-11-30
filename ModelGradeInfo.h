#ifndef MODEL_GRADE_INFO_H
#define MODEL_GRADE_INFO_H

class ModelGradeInfo
{
    public:
        int grade, type_id, model_id;
        ModelGradeInfo (int grade, int type_id, int model_id) : grade(grade), type_id(type_id),
                                                            model_id(model_id) {}
        ~ModelGradeInfo() = default;

    // ======= Operators =======
    bool operator<(const ModelGradeInfo& model_info_two);
    bool operator== (const ModelGradeInfo& model_info_two);
};

#endif //MODEL_GRADE_INFO_H