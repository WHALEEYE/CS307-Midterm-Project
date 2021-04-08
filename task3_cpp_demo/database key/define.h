#ifndef DEFINE_H
#define DEFINE_H

#define term shared_ptr<vector<shared_ptr<baseItem>>>
#define form shared_ptr<vector<term>>

#define baseItemToNum(T) dynamic_pointer_cast<NumItem>(T)
#define baseItemToFloat(T) dynamic_pointer_cast<FloatItem>(T)
#define baseItemToChar(T) dynamic_pointer_cast<CharItem>(T)

#define baseGetNum(T) baseItemToNum(T)->getItem()
#define baseGetFloat(T) baseItemToFloat(T)->getItem()
#define baseGetChar(T) baseItemToChar(T)->getItem()

#define pNumItem shared_ptr<NumItem>
#define pFloatItem shared_ptr<FloatItem>
#define pCharItem shared_ptr<CharItem>
#define pBaseItem shared_ptr<baseItem>

#define NumItem Titem<long>
#define FloatItem Titem<double>
#define CharItem Titem<string>

#define pNormalKey shared_ptr<normalKey>

#define NumMap multimap<long long int,term>
#define FloatMap multimap<double,term>
#define CharMap multimap<string,term>

#define pNumMap shared_ptr<NumMap>
#define pFloatMap shared_ptr<FloatMap>
#define pCharMap shared_ptr<CharMap>

#define pDataMatrix shared_ptr<dataMatrix>

#endif // DEFINE_H
