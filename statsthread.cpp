#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;


int min_val;
int max_val;
double avg_val; 

void* maximum(void* args)
{
    vector<int> arr = *(vector<int>*) args;
    max_val = arr[0];
    
    for(int i = 1; i < arr.size(); i++)
    {
        if(arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }
    
    pthread_exit(NULL);
}

void* minimum(void* args)
{
    vector<int> arr = *(vector<int>*) args;
    min_val = arr[0];
    
    for(int i = 1; i < arr.size(); i++)
    {
        if(arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }
    
    pthread_exit(NULL);
}

void* average(void* args)
{
    vector<int> arr = *(vector<int>*) args;
    double sum = 0; // Changed to double
    for(int i = 0; i < arr.size(); i++)
    {
        sum += arr[i];
    }
    avg_val = sum / arr.size();
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t id, id2, id3;

    vector<int> array = {90, 81, 78, -95, 79, 72, 85};

    pthread_create(&id, NULL, &maximum, &array);
    pthread_create(&id2, NULL, &average, &array);
    pthread_create(&id3, NULL, &minimum, &array);

    pthread_join(id, NULL);
    pthread_join(id2, NULL);
    pthread_join(id3, NULL);

    cout << "The avg is: " << avg_val << "\n";
    cout << "The max is: " << max_val << "\n";
    cout << "The min is: " << min_val << "\n";

    return 0;
}
