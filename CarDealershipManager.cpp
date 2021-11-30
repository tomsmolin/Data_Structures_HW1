#include "CarDealershipManager.h"

StatusType CarDealershipManager::AddCarType(int typeID, int numOfModels)
{
    if (types_tree.find(typeID) != nullptr)
    {
        return FAILURE;
    }

    Node<int, TypeInfo>* type_node = types_tree.insert(typeID, TypeInfo(numOfModels));
    if (!type_node)
    {
        return ALLOCATION_ERROR;
    }

    type_node->val.grades_models = new int[numOfModels];
    type_node->val.sales_models = new int[numOfModels];

    if (!type_node->val.grades_models || !type_node->val.sales_models)
    {
        return ALLOCATION_ERROR;
    }

    Node<int, AvlTreeWrap>* zeros_tree_type_node = zero_graded_tree.insert(typeID, AvlTreeWrap());
    if (!zeros_tree_type_node)
    {
        return ALLOCATION_ERROR;
    }

    zeros_tree_type_node->val.sub_zero_tree.root = createSortedModelsTree(nullptr, typeID, 0, numOfModels - 1);
    if (!zeros_tree_type_node->val.sub_zero_tree.root)
    {
        return ALLOCATION_ERROR;
    }
    zeros_tree_type_node->val.minimal_zero_model_node = zeros_tree_type_node->val.sub_zero_tree.getMin();

    initializeArrayToZero(type_node->val.grades_models, numOfModels);
    initializeArrayToZero(type_node->val.sales_models, numOfModels);

    minimal_grade_model = models_grade_tree.getMin();
    minimal_zero_type = zero_graded_tree.getMin();
    zero_graded_models_num += numOfModels;
    total_models_num += numOfModels;

    return SUCCESS;
}

StatusType CarDealershipManager::RemoveCarType(int typeID)
{
    Node<int, TypeInfo>* type_node = types_tree.find(typeID);
    if (!type_node)
    {
        return FAILURE;
    }
    
    RemoveCarTypeFromZeroesTree(typeID);
    RemoveCarTypeNodesFromSalesAndGradesTree(type_node, typeID);
    total_models_num -= type_node->val.models_num;
    RemoveCarTypeFromTypesTree(type_node, typeID);

    minimal_grade_model = models_grade_tree.getMin();
    minimal_zero_type = zero_graded_tree.getMin();
    if (most_popular_model == nullptr)
    {
        most_popular_model = models_sales_tree.getMax();
    }

    return SUCCESS;
}

StatusType CarDealershipManager::SellCar(int typeID, int modelID)
{
    Node<int, TypeInfo>* type_node = types_tree.find(typeID);
    if (!type_node)
    {
        return FAILURE;
    }

    if (modelID >= type_node->val.models_num)
    {
        return FAILURE;
    }

    int old_grade = type_node->val.grades_models[modelID];
    int new_grade = old_grade + SALE_POINTS;
    int old_sales_num = type_node->val.sales_models[modelID];
    int new_sales_num = old_sales_num + 1;
    
    updateBestSellerOfType(type_node, modelID, new_sales_num);
    StatusType update_res_one = updateModelsGradesTrees(type_node, modelID, old_grade, new_grade);
    StatusType update_res_two = updateModelsSalesTree(type_node, modelID, old_sales_num, new_sales_num);
    if (update_res_one != SUCCESS || update_res_two != SUCCESS)
    {
        return ALLOCATION_ERROR;
    }

    type_node->val.grades_models[modelID] = new_grade;
    type_node->val.sales_models[modelID] = new_sales_num;
    most_popular_model = models_sales_tree.getMax();

    return SUCCESS;
}

StatusType CarDealershipManager::MakeComplaint(int typeID, int modelID, int t)
{
    Node<int, TypeInfo>* type_node = types_tree.find(typeID);
    if (type_node == nullptr)
    {
        return FAILURE;
    }
    if(type_node->val.models_num <= modelID)
    {
        return FAILURE;
    }

    int prev_grade = type_node->val.grades_models[modelID];
    type_node->val.grades_models[modelID] = prev_grade - (100/t);
    int curr_grade = type_node->val.grades_models[modelID];

    if(curr_grade == 0)
    {
       return updateModelGradeToZero(typeID, modelID, prev_grade, curr_grade);
    }
    
    else if(prev_grade == 0)
    {
        return updateModelGradeFromZero(typeID,modelID, prev_grade, curr_grade);
    }

    else
    {
        return updateNonZeroModelGrade(typeID, modelID, prev_grade, curr_grade);
    }
}

StatusType CarDealershipManager::GetBestSellerModelByType(int typeID, int *modelID)
{
    if (typeID == 0)
    {
        if(types_tree.root == nullptr)
        {
            return FAILURE;
        }
        if(models_sales_tree.root == nullptr)
        {
            *modelID = 0;
            return SUCCESS;
        }
        
        *modelID = most_popular_model->key.model_id;
        return SUCCESS;
    }

    Node<int, TypeInfo>* type_node = types_tree.find(typeID);
    if (type_node == nullptr)
    {
        return FAILURE;
    }
    *modelID = type_node->val.best_seller_model;
    return SUCCESS;
}

StatusType CarDealershipManager::GetWorstModels(int numOfModels , int *types, int *models)
{
    if (total_models_num < numOfModels)
    {
        return FAILURE;
    }
    bool predecessor_negative = false;
    int counter = 0;

    Node<ModelGradeInfo, int>* iterator = minimal_grade_model;
    if(!iterator || iterator->key.grade > 0)
    {
        fillArraysFromBigZeroTree_aux(numOfModels, types, models, &counter);
        if (numOfModels == counter)
        {
            return SUCCESS;
        }
    }

    while (counter < numOfModels && iterator != nullptr)
    {
        if (iterator->key.grade < 0)
        {
            predecessor_negative = true;
            assignInformation(iterator, &counter, types, models);
        }
        if (iterator->key.grade > 0 && predecessor_negative)
        {
            
            if (zero_graded_models_num > 0)
            {
                fillArraysFromBigZeroTree_aux(numOfModels, types, models, &counter);
                if (numOfModels == counter)
                {
                    return SUCCESS;
                }
            }
            predecessor_negative = false;
        }
        if(iterator->key.grade > 0 && !(predecessor_negative))
        {
            assignInformation(iterator, &counter, types, models);
        }

        if (iterator->right != nullptr)
        {
            fillArraysFromGradesTree(iterator->right, numOfModels, types, models, &counter, &predecessor_negative);
        }
        iterator = iterator->parent;
    }
    return SUCCESS;
}

void CarDealershipManager::Quit()
{
    minimal_zero_type = nullptr;
    minimal_grade_model = nullptr;
    most_popular_model = nullptr;
    models_grade_tree.deleteTree(models_grade_tree.root);
    models_sales_tree.deleteTree(models_sales_tree.root);

    inOrderDeleteSubTrees(zero_graded_tree.root);
    zero_graded_tree.deleteTree(zero_graded_tree.root);
    
    inOrderDeleteArrays(types_tree.root);
    types_tree.deleteTree(types_tree.root);
}

void CarDealershipManager::RemoveCarTypeFromZeroesTree(int typeID)
{
    Node<int, AvlTreeWrap>* zeros_tree_type_node = zero_graded_tree.find(typeID);
    if (zeros_tree_type_node)
    {
        zeros_tree_type_node->val.sub_zero_tree.deleteTree(zeros_tree_type_node->val.sub_zero_tree.root);
        zeros_tree_type_node->val.minimal_zero_model_node = nullptr;
        zero_graded_tree.remove(typeID);
    }
}

void CarDealershipManager::RemoveCarTypeNodesFromSalesAndGradesTree(Node<int, TypeInfo>* type_node, int typeID)
{
    int zero_graded_models_counter = 0;
    for (int model = 0; model < type_node->val.models_num; model++)
    {
        int curr_grade;
        int curr_sales;
        curr_grade = type_node->val.grades_models[model];
        curr_sales = type_node->val.sales_models[model];

        if (curr_grade != 0)
        {
            ModelGradeInfo model_grades_node(curr_grade, typeID, model);
            models_grade_tree.remove(model_grades_node);
        }
        else
        {
            zero_graded_models_counter++;
        }

        if (curr_sales != 0)
        {
            ModelSaleInfo model_sales_node(curr_sales, typeID, model);
            if (most_popular_model)
            {
                if (model_sales_node == most_popular_model->key)
                {
                    most_popular_model = nullptr;
                }
            }
            models_sales_tree.remove(model_sales_node);
        }
    }
    zero_graded_models_num -= zero_graded_models_counter;
}

void CarDealershipManager::RemoveCarTypeFromTypesTree(Node<int, TypeInfo>* type_node, int typeID)
{
    delete[] type_node->val.grades_models;
    delete[] type_node->val.sales_models;
    types_tree.remove(typeID);
}

void CarDealershipManager::updateBestSellerOfType(Node<int, TypeInfo>* type_node, int modelID, int new_sales_num)
{
    if (type_node->val.best_seller_sales < new_sales_num)
    {
        type_node->val.best_seller_sales = new_sales_num;
        type_node->val.best_seller_model = modelID;
    }

    else if (type_node->val.best_seller_sales == new_sales_num)
    {
        if (modelID < type_node->val.best_seller_model)
        {
            type_node->val.best_seller_sales = new_sales_num;
            type_node->val.best_seller_model = modelID;
        }
    }
}

StatusType CarDealershipManager::updateModelsGradesTrees(Node<int, TypeInfo>* type_node, int modelID,
                                                                    int old_grade, int new_grade)
{
    int typeID = type_node->key;
    ModelGradeInfo grade_key(old_grade, typeID, modelID);
    if (old_grade != 0)
    {
        
        if (new_grade != 0)
        {
            return updateNonZeroModelGrade(typeID, modelID, old_grade, new_grade);
        }
        else
        {
            return updateModelGradeToZero(typeID, modelID, old_grade, new_grade);
        }
    }
    else
    {
        return updateModelGradeFromZero(typeID, modelID, old_grade, new_grade);
    }
    return SUCCESS;
}

StatusType CarDealershipManager::updateModelsSalesTree(Node<int, TypeInfo>* type_node, int modelID,
                                                              int old_sales_num, int new_sales_num)
{
    int typeID = type_node->key;
    ModelSaleInfo sale_key(old_sales_num, typeID, modelID);
    if (old_sales_num != 0)
    {
        models_sales_tree.remove(sale_key);
        sale_key.sales_num = new_sales_num;
        if (models_sales_tree.insert(sale_key, modelID) == nullptr)
        {
            return ALLOCATION_ERROR;
        }
    }
    else
    {
        sale_key.sales_num = new_sales_num;
        if (models_sales_tree.insert(sale_key, modelID) == nullptr)
        {
            return ALLOCATION_ERROR;
        }
    }
    return SUCCESS;
}

StatusType CarDealershipManager::updateModelGradeToZero(int typeID, int modelID, int prev_grade, int curr_grade)
{
    ModelGradeInfo key(prev_grade, typeID, modelID);
    models_grade_tree.remove(key);
    minimal_grade_model = models_grade_tree.getMin();
    key.grade = curr_grade;

    Node<int, AvlTreeWrap>* zeros_tree_type_node = zero_graded_tree.find(typeID);
    if(!zeros_tree_type_node)  
    {
        if (zero_graded_tree.insert(typeID, AvlTreeWrap()) == nullptr)
        {
            return ALLOCATION_ERROR;
        }
        zeros_tree_type_node = zero_graded_tree.find(typeID);
    }
    
    if (zeros_tree_type_node->val.sub_zero_tree.insert(key, modelID) == nullptr)
    {
        return ALLOCATION_ERROR;
    }
    zeros_tree_type_node->val.minimal_zero_model_node = zeros_tree_type_node->val.sub_zero_tree.getMin();
    minimal_zero_type = zero_graded_tree.getMin();
    zero_graded_models_num++;
    return SUCCESS;
}

StatusType CarDealershipManager::updateModelGradeFromZero(int typeID, int modelID, int prev_grade, int curr_grade)
{
    ModelGradeInfo key(prev_grade, typeID, modelID);
    Node<int, AvlTreeWrap>* zeros_tree_type_node = zero_graded_tree.find(typeID);
    zeros_tree_type_node->val.sub_zero_tree.remove(key);
    zeros_tree_type_node->val.minimal_zero_model_node = zeros_tree_type_node->val.sub_zero_tree.getMin();
   
   if(zeros_tree_type_node->val.sub_zero_tree.root == nullptr)
    {
        zeros_tree_type_node->val.minimal_zero_model_node = nullptr;
        zero_graded_tree.remove(typeID);
        minimal_zero_type = zero_graded_tree.getMin();
    }

    key.grade = curr_grade;
    if (models_grade_tree.insert(key, modelID) == nullptr)
    {
        return ALLOCATION_ERROR;
    }
    minimal_grade_model = models_grade_tree.getMin();
    zero_graded_models_num--;
    return SUCCESS;
}

StatusType CarDealershipManager::updateNonZeroModelGrade(int typeID, int modelID, int prev_grade, int curr_grade)
{
    ModelGradeInfo key(prev_grade, typeID, modelID);
    models_grade_tree.remove(key);
    key.grade = curr_grade;
    if (models_grade_tree.insert(key, modelID) == nullptr)
    {
        return ALLOCATION_ERROR;
    }
    minimal_grade_model = models_grade_tree.getMin();
    return SUCCESS;
}

void CarDealershipManager::fillArraysFromBigZeroTree_aux(int numOfModels, int* types, int* models, int* counter)
{
    Node<int, AvlTreeWrap>* iterator = minimal_zero_type;
    while (*counter < numOfModels && iterator != nullptr)
    {
        fillArraysFromTypesZeroes_aux(iterator->val.minimal_zero_model_node, numOfModels, types, models, counter);
        if (iterator->right)
        {
            fillArraysFromBigZeroTree(iterator->right, numOfModels, types, models, counter);
        }
        iterator = iterator->parent;
    }
}

void CarDealershipManager::fillArraysFromBigZeroTree(Node<int, AvlTreeWrap>* node, int numOfModels,
    int* types, int* models, int* counter)
{
    if (node == nullptr)
    {
        return;
    }
    if (numOfModels == *counter)
    {
        return;
    }

    fillArraysFromBigZeroTree(node->left, numOfModels, types, models, counter); 

    fillArraysFromTypesZeroes_aux(node->val.minimal_zero_model_node, numOfModels, types, models, counter);

    fillArraysFromBigZeroTree(node->right, numOfModels, types, models, counter);
}

void CarDealershipManager::fillArraysFromTypesZeroes_aux(Node<ModelGradeInfo, int>* node, int numOfModels,
                                                                int* types, int* models, int* counter)
{
    if (node == nullptr)
    {
        return;
    }
    if (numOfModels == *counter)
    {
        return;
    }

    Node<ModelGradeInfo, int>* iterator = node;
    while (*counter < numOfModels && iterator != nullptr)
    {
        assignInformation(iterator, counter, types, models);
        if (iterator->right)
        {
            fillArraysFromTypesZeroes(iterator->right, numOfModels, types, models, counter);
            if (numOfModels == *counter)
            {
                return;
            }
        }
        iterator = iterator->parent;
    }
}

void CarDealershipManager::fillArraysFromTypesZeroes(Node<ModelGradeInfo, int>* node,  int numOfModels,
                                                                     int* types, int* models, int* counter)
{
    if (node == nullptr)
    {
        return;
    }

    if (numOfModels == *counter)
    {
        return;
    }

    fillArraysFromTypesZeroes(node->left, numOfModels, types, models, counter);
    
    if (numOfModels == *counter)
    {
        return;
    }

    assignInformation(node, counter, types, models);

    fillArraysFromTypesZeroes(node->right, numOfModels, types, models, counter);
}

void CarDealershipManager::fillArraysFromGradesTree(Node<ModelGradeInfo, int>* node, int numOfModels,
    int* types, int* models, int* counter, bool* predecessor_negative)
{
    if (node == nullptr)
    {
        return;
    }

    if (numOfModels == *counter)
    {
        return;
    }

    fillArraysFromGradesTree(node->left, numOfModels, types, models, counter, predecessor_negative);
    
    if (numOfModels == *counter)
    {
        return;
    }

    if (node->key.grade < 0)
    {
        assignInformation(node, counter, types, models);
    }

    if (node->key.grade > 0 && *predecessor_negative)
    {
        if (zero_graded_models_num > 0)
        {
            fillArraysFromBigZeroTree_aux(numOfModels, types, models, counter);
            if (numOfModels == *counter)
            {
                return;
            }
        }
        *predecessor_negative = false;
    }

    if (node->key.grade > 0 && !(*predecessor_negative))
    {
        assignInformation(node, counter, types, models);
    }
    
    fillArraysFromGradesTree(node->right, numOfModels, types, models, counter, predecessor_negative);
}

void CarDealershipManager::assignInformation(Node<ModelGradeInfo, int>* node, int* counter, int* types, int* models)
{
    models[*counter] = node->key.model_id;
    types[*counter] = node->key.type_id;
    (*counter)++;
}

Node <ModelGradeInfo, int>* createSortedModelsTree(Node <ModelGradeInfo, int>* node, int typeID, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int mid =  (start + end) / 2;

    Node <ModelGradeInfo, int>* root = new Node<ModelGradeInfo, int>(ModelGradeInfo(0, typeID, mid), mid);
    root->parent = node;
    root->left = createSortedModelsTree(root, typeID, start, mid - 1);
    root->right = createSortedModelsTree(root, typeID, mid + 1, end);

    AVLTree<ModelGradeInfo, int> tree;
    tree.updateHeights(root);
     
    return root;
}

void initializeArrayToZero(int* grades_models, int numOfModels)
{
    for (int i = 0 ; i < numOfModels; i++)
    {
        grades_models[i] = 0;
    }
}

void inOrderDeleteArrays(Node <int, TypeInfo>* node)
{
    if (node == nullptr)
    {
        return;
    }
    inOrderDeleteArrays(node->left);
    delete[] node->val.grades_models;
    delete[] node->val.sales_models;
    inOrderDeleteArrays(node->right);
}

void inOrderDeleteSubTrees(Node <int, AvlTreeWrap>* node)
{
    if (node == nullptr)
    {
        return;
    }
    inOrderDeleteSubTrees(node->left);
    node->val.sub_zero_tree.deleteTree(node->val.sub_zero_tree.root);
    inOrderDeleteSubTrees(node->right);
}