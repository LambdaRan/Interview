#include <iostream>
#include <vector>
#include <initializer_list>
#include <memory>

using namespace std;

class TA 
{
public:
    TA(int val) : value_(val)
    {
        cout << "TA("<< value_<< ")" << endl;
    }
    ~TA() 
    {
        cout << "~TA(" << value_ << ")"<< endl;
    }
    int value_;
};


int main()
{
    std::shared_ptr<vector<TA>> g_spv = std::make_shared<vector<TA>>();
    g_spv->push_back(std::move(TA(1)));
    g_spv->push_back(std::move(TA(2)));
    // 1 2
    cout << "0. g_spv.use_count:" << g_spv.use_count() << " vector.size:" << g_spv->size() << endl;
    {// 模拟读
        std::shared_ptr<vector<TA>> sp2 = g_spv;
        // 2 2
        cout << "1. g_spv.use_count:" << g_spv.use_count() << " vector.size:" << g_spv->size() << " ";
        cout << "sp2.use_count:" << sp2.use_count() << endl;
        // 模拟写
        if (!g_spv.unique()) // 不是独占，需要复制一份
        {
            //std::shared_ptr<vector<TA>> newData(new vector<TA>(*g_spv));
            //g_spv.swap(newData);
            g_spv.reset(new vector<TA>(*g_spv));
            g_spv->push_back(std::move(TA(3)));
            // 1 3
            cout << "2. g_spv.use_count:" << g_spv.use_count() << " vector.size:" << g_spv->size() << " ";
            cout << "sp2.use_count:" << sp2.use_count() << endl;
        } // 旧析构
        // 1 3
        cout << "3. g_spv.use_count:" << g_spv.use_count() << " vector.size:" << g_spv->size() << " ";
        cout << "sp2.use_count:" << sp2.use_count() << endl;
    }

    // 1 3
    cout << "4. g_spv.use_count:" << g_spv.use_count() << " vector.size:" << g_spv->size() << endl;

    return 0;
}