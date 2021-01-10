#include<iostream>
#include<vector>
#include<omp.h>
#include<sys/time.h>
using namespace std;
    

vector<string> generateParenthesis(int n) {
    if (!n) return {""};

    vector<string> parens, inner, outer;
    int i = 0;

    for (int i = 0; i < n; i++) {
        #pragma omp task
        inner = generateParenthesis(i); //#1
        #pragma omp task
        outer = generateParenthesis(n - 1 - i); //#2

        #pragma omp taskwait
        for (int j = 0; j < inner.size(); ++j) {
            for (int k = 0; k < outer.size(); ++k) {
                parens.push_back("(" + inner[j] + ")" + outer[k]);
            }
        }
    }
    return parens;
}

int main(){
    float sum = 0;
    for (int i = 0; i < 100; i++)
    {
        struct timeval stime,etime,gap;

        gettimeofday(&stime,NULL);
        #pragma omp parallel num_threads(32)
        {
            #pragma omp single
            generateParenthesis(10);
        }
        gettimeofday(&etime,NULL);

        gap.tv_sec = etime.tv_sec - stime.tv_sec;
        gap.tv_usec = etime.tv_usec - stime.tv_usec;

        if(gap.tv_usec < 0){
            gap.tv_sec = gap.tv_sec-1; 
            gap.tv_usec = gap.tv_usec + 1000000;
        }
        sum += gap.tv_usec;
    }
    cout << sum/100 << endl;
}