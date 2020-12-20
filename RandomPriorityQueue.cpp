#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;
   
template<class T>
class RandomPriorityQueue
{
public:
    RandomPriorityQueue(void)
    {
        srand (time(NULL));
    }
    
    ~RandomPriorityQueue(void)
    {
    }
    
    // Randomly selects an element and removes it from the queue with the probability of item i being selected is priority_i.
    T Pop()
    {
        // find the item idx to pop 
        int idx = getIdxByProb();
 
        // pop from queue
        T toPop = vQueue_[idx];
        swap(vQueue_[idx], vQueue_.back());
        vQueue_.pop_back();
        
        // pop from vDPrior
        double newSumDPrior = sumDPrior_ - vDPrior_[idx];
        swap(vDPrior_[idx], vDPrior_.back());
        vDPrior_.pop_back();
        
        // pop from vCumProb
        swap(vCumProb_[idx], vCumProb_.back());
        vCumProb_.pop_back();
        for (int i; i < vCumProb_.size(); ++i)
        {
            if (i < idx)
                vCumProb_[i] = vCumProb_[i] * sumDPrior_ / newSumDPrior;
            else
                vCumProb_[i] = vCumProb_[i-1] + vDPrior_[i] / newSumDPrior;            
        }
        sumDPrior_ = newSumDPrior;
        
        return toPop;
    }
    
    // Inserts "value" to the queue with the given priority dPriority cannot be negative.
    void Insert(double _dPriority, const T& _value)
    {
        // update vCumProb with new dPriority
        double newSumDPrior = sumDPrior_ + _dPriority;
        for (int i=0; i < vCumProb_.size(); ++i)
        {
            vCumProb_[i] = vCumProb_[i] * sumDPrior_ / newSumDPrior;
        }
        vCumProb_.push_back(1.0);
        
        // update vDPrior with new dPriority
        vDPrior_.push_back(_dPriority);
        sumDPrior_ = newSumDPrior;
          
        // insert the new item
        vQueue_.push_back(_value);    
    }

    // Return if the Queue is empty.
    inline bool Empty(void) 
    {
        return vQueue_.empty();
    }
    
    // Return the size of the Queue.
    inline size_t Size(void)
    {
        return vQueue_.size();
    }
    
    // Print the queue
    void PrintQueue(void)
    {
        for (int i=0; i < vQueue_.size(); ++i)
        {
            // note here it needs a generalized display function for class T
            cout << vQueue_[i] << " ";
        }
        cout << endl;
    }
    
private:
    vector<T>      vQueue_;
    vector<double> vDPrior_;
    vector<double> vCumProb_;
    double         sumDPrior_;
      
    // Find the min idx with cumProb > random num r.
    int getIdxByProb(void)
    {
        double r = ((double) rand() / (RAND_MAX));
        int idx = lower_bound(vCumProb_.begin(), vCumProb_.end(), r) - vCumProb_.begin();
        return idx;
    }
};
    

int main()
{
    enum Ops {CHK_EMP, CHK_SZ, INSERT, POP};
    RandomPriorityQueue<double> rpq;
    while (1)
    {
        int op;
        cout << "Enter your operation: 0-CHK_EMP, 1-CHK_SZ, 2-INSERT, 3-POP" << endl;
        cin >> op;
        switch (op)
        {
            case CHK_EMP:
            {
                string emp = rpq.Empty() ? "Empty" : "not Empty";
                cout << "My queue is " << emp << endl;
                break;
            }
            case CHK_SZ:
            {
                cout << "My queue size = " << rpq.Size() << endl;
                break;                           
            }
            case INSERT:
            {                               
                cout << "Input (dProiorty, value): " << endl;
                double dp, val;
                cin >> dp >> val;
                rpq.Insert(dp, val);
                rpq.PrintQueue();
                break;
            }
            case POP:   
            {
                double popItem = rpq.Pop();
                cout << "Item " << popItem << " is poped" << endl;
                rpq.PrintQueue();
                break;
            }
            default:
            {
                cout << "Unknow operation" << endl;
                break;
            }
        }        
    }
    
    return 0;
}
