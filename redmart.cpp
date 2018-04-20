#include<iostream>
#include<cstdio>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>

using namespace std;

//macro for simplifying pair<int, int> intialization
#define ii pair<int,int>

//constants
const string fileName = "products.csv"; //change based on file location
const int toteDim[3] = {30, 35, 45}; //sorted
const int maxVolume = 30*35*45;

//Data structure
class Product {
    /*
    Data structure for containing a product record
    */
    public:
        //public properties
        int id;
        int price;
        int volume;//length * width * height
        int weight;

        //constructor
        Product(int id, int price, int volume, int weight) {
            this->id = id;
            this->price = price;
            this->volume = volume;
            this->weight = weight;
        } 
};

bool dimCompatible(int productDim[3]) {
    /*
    check if the product dimmension is compatible
    */
    bool compatible = true;

    for(int i = 0; i < 3; i++)
        compatible &= (productDim[i] <= toteDim[i]);
    
    return compatible;
}

void readCSVFile(string fileName, vector<Product> &productVector) {
    /*
    Read products CSV file and filter out incompatible products
    */
    ifstream productCSV(fileName);
    
    if (!productCSV.is_open()) {
        //File does not exist
        cout<<"ERROR, FILE DOES NOT EXIST!"<<endl;
        return;
    }

    //read each lines of CSV
    int incompatibleCount = 0;
    string id, price, length, width, height, weight;
    while (getline(productCSV, id, ',') &&
            getline(productCSV, price, ',') &&
            getline(productCSV, length, ',') &&
            getline(productCSV, width, ',') &&
            getline(productCSV, height, ',') &&
            getline(productCSV, weight, '\n')){
        
        int length_num = stoi(length);
        int width_num = stoi(width);
        int height_num = stoi(height);

        //compatibility check
        int productDim[3] = {length_num, width_num, height_num};
        sort(productDim, productDim+3);
        bool compatible = dimCompatible(productDim);
        if (!compatible) {
            incompatibleCount++;
            continue;
        }
        
        //create product data structure
        int volume = length_num * width_num * height_num;
        Product product(stoi(id), stoi(price), volume, stoi(weight));

        productVector.push_back(product);
    }
    productCSV.close();
    
    //Product statistics
    cout<<"Number of Compatible Product: "<<productVector.size()<<endl;
    cout<<"Number of Incompatible Product: "<<incompatibleCount<<endl<<endl;

    return;
}

void redmartKnapsack(vector<Product> &productVector, vector<int> &res) {
    /*
    Solving problem using custom Knapsack Dynamic Programming. Since we need to maximize total price and minimize total weight, 2 DP memoization are used.
    Ideally, N*V arrays are needed for DP memoization where N is number of product and V is max tote volume. However, there is a need to reduce DP memoization space
    from N*V to V (only concern to the previous product number constraint) since the immediate allocation of N*W triggered stack overflow in C++. 
    So, there is a need for another DP memoization for saving last picked product and adjacency list for backtracking. The adjcency list add edges from i th product to
    i-x product with weight edge possible volume state of when choosing i-x th product.
    Time complexity is O(N*V) and space complexity is O(N*V) since adjacency list add edges from i th product to i-x th product with different volume.   
    */

    //initialize dp memoization array
    int n = productVector.size();
    int dp_price[maxVolume+1];
    int dp_weight[maxVolume+1];
    int dp_idx[maxVolume+1];    
    vector<ii> adj[n+1];

    memset(dp_price, 0, sizeof(dp_price));
    memset(dp_weight, 0, sizeof(dp_weight));
    memset(dp_idx, -1, sizeof(dp_idx));


    //fill dp in bottom up manner
    for(int i = 0; i < n; i++) {
        Product product = productVector[i];
        for(int v = maxVolume; v >= product.volume ; v--) {
            int prevVolume = v - product.volume;
            int newSumPrice = dp_price[prevVolume] + product.price;
            int newSumWeight = dp_weight[prevVolume] + product.weight;
            if (dp_price[v] == newSumPrice && newSumWeight < dp_weight[v]) {
                // pick product that yield minimal weight
                dp_weight[v] = newSumWeight;
                if (dp_idx[prevVolume] == -1) {
                        dp_idx[v] = i;
                        continue;
                }
                adj[i].push_back(ii(prevVolume, dp_idx[prevVolume]));
                dp_idx[v] = i;
            }
            else if (dp_price[v] < newSumPrice) {
                //pick product that yield maximum total price
                dp_price[v] = newSumPrice;
                dp_weight[v] = newSumWeight;
                if (dp_idx[prevVolume] == -1) {
                        dp_idx[v] = i;
                        continue;
                }
                adj[i].push_back(ii(prevVolume, dp_idx[prevVolume]));
                dp_idx[v] = i;
            }
        }
        reverse(adj[i].begin(), adj[i].end()); //sort based on increasing volume
    }

    int curVolume = maxVolume;
    int curIdx = dp_idx[curVolume];

    int totalId = 0;
    if (curIdx == -1) {
        //no product choosen
        cout<<"Error, no product compatible\n";
        return;
    }

    //backtrack
    res.push_back(productVector[curIdx].id);
    totalId += productVector[curIdx].id;
    curVolume -= productVector[curIdx].volume;
    while(adj[curIdx].size() > 0 && curVolume > 0) {
        //search previous product with a certain total volume state using binary search
        vector<ii>::iterator low = lower_bound(adj[curIdx].begin(), adj[curIdx].end(), ii(curVolume, -1));
        if (low == adj[curIdx].end()) //no product found
            break;
        int targetIdx = low - adj[curIdx].begin();
        if (adj[curIdx][targetIdx].first > curVolume) // no product found
            break;
        //add product to result
        curIdx = adj[curIdx][targetIdx].second;
        res.push_back(productVector[curIdx].id);
        totalId += productVector[curIdx].id;
        curVolume -= productVector[curIdx].volume;
    }
    reverse(res.begin(), res.end());
    
    cout<<"Selected Product ID: "<<endl;
    for(int i: res)
        cout<<i<<' ';
    cout<<endl<<endl;
    cout<<"Sum of ID: "<<totalId<<endl;
}

int main() {
    //load data 
    vector<Product> productVector;
    readCSVFile(fileName, productVector);
    
    //solve problem
    vector<int> res;
    redmartKnapsack(productVector, res);
    return 0;
}