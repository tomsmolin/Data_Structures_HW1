#ifndef CAR_DS_MANAGER_H
#define CAR_DS_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "AvlTreeWrap.h"
#include "TypeInfo.h"
#include "ModelGradeInfo.h"
#include "ModelSaleInfo.h"

#define SALE_POINTS 10;

Node <ModelGradeInfo, int>* createSortedModelsTree(Node <ModelGradeInfo, int>* node, int typeID, int start, int end);
void initializeArrayToZero(int* grades_models, int numOfModels);
void inOrderDeleteArrays(Node <int, TypeInfo>* node);
void inOrderDeleteSubTrees(Node <int, AvlTreeWrap>* node);

class CarDealershipManager
{
public:
    CarDealershipManager() : types_tree(), zero_graded_tree(), models_grade_tree(), models_sales_tree(),
                             minimal_zero_type(nullptr), minimal_grade_model(nullptr), most_popular_model(nullptr),
                             total_models_num(0), zero_graded_models_num(0) {}
    ~CarDealershipManager() = default;

    // Trees
    AVLTree<int, TypeInfo> types_tree;
    AVLTree<int, AvlTreeWrap> zero_graded_tree;
    AVLTree<ModelGradeInfo, int> models_grade_tree;
    AVLTree<ModelSaleInfo, int> models_sales_tree;

    // Pointers to extremums
    Node<int, AvlTreeWrap>* minimal_zero_type;
    Node<ModelGradeInfo, int>* minimal_grade_model;
    Node<ModelSaleInfo, int>* most_popular_model;

    // Local Variables
    int total_models_num;
    int zero_graded_models_num;
    
    // ========= Methods ==========
   StatusType AddCarType(int typeID, int numOfModels);
   StatusType RemoveCarType(int typeID);
   StatusType SellCar(int typeID, int modelID);
   StatusType MakeComplaint(int typeID, int modelID, int t);
   StatusType GetBestSellerModelByType(int typeID, int *modelID);
   StatusType GetWorstModels(int numOfModels , int *types, int *models);
   void Quit();

private:
    void RemoveCarTypeFromZeroesTree (int typeID);
    void RemoveCarTypeNodesFromSalesAndGradesTree (Node<int, TypeInfo>* type_node, int typeID);
    void RemoveCarTypeFromTypesTree (Node<int, TypeInfo>* type_node, int typeID);

    void updateBestSellerOfType (Node<int, TypeInfo>* type_node, int modelID, int new_sales_num);
    StatusType updateModelsGradesTrees (Node<int, TypeInfo>* type_node, int modelID, int old_grade, int new_grade);
    StatusType updateModelsSalesTree (Node<int, TypeInfo>* type_node, int modelID, int old_sales_num, int new_sales_num);

    StatusType updateModelGradeToZero(int typeID, int modelID, int prev_grade, int curr_grade);
    StatusType updateModelGradeFromZero(int typeID, int modelID, int prev_grade, int curr_grade);
    StatusType updateNonZeroModelGrade(int typeID, int modelID, int prev_grade, int curr_grade);

    void fillArraysFromBigZeroTree_aux(int numOfModels, int* types, int* models, int* counter);
    void fillArraysFromBigZeroTree(Node<int, AvlTreeWrap>* node, int numOfModels,
                                          int* types, int* models, int* counter);
    void fillArraysFromTypesZeroes_aux(Node<ModelGradeInfo, int>* node, int numOfModels,
                                                  int* types, int* models, int* counter);
    void fillArraysFromTypesZeroes(Node<ModelGradeInfo, int>* node, int numOfModels,
                                             int* types, int* models, int* counter);  
    void fillArraysFromGradesTree(Node<ModelGradeInfo, int>* node, int numOfModels,
                                  int* types, int* models, int* counter, bool* predecessor_negative);
    void assignInformation(Node<ModelGradeInfo, int>* node, int* counter, int* types, int* models);
};

#endif //CAR_DS_MANAGER_H