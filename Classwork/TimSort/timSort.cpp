#include<iostream>
#include<algorithm>
#include<cstdlib>
//#include<bits/stdc++.h>
using namespace std;
const int RUN = 32; 
void insertionSort(int arr[], int left, int right);
void merge(int arr[], int l, int m, int r);
void timSort(int arr[], int n);
void printArray(int arr[], int n);

int main(){
   int arr[] = {-2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8, -14, 12};
   int n = sizeof(arr)/sizeof(arr[0]);
   cout << "n = " << n << endl;
   cout << "The Original array- " << endl;
   printArray(arr, n);

   timSort(arr, n);
   cout<<"After Sorting Array Using TimSort Algorithm- " << endl;
   printArray(arr, n); 
   return 0;
}

void insertionSort(int arr[], int left, int right) 
{
   for (int i = left + 1; i <= right; i++){
      int temp = arr[i];
      int j = i - 1;
      while (j >= left && temp < arr[j]){
         arr[j+1] = arr[j];
         j--;
      }
      arr[j+1] = temp;
   }
}

void merge(int arr[], int l, int m, int r) {
   int len1 = m - l + 1, len2 = r - m;
   int left[len1], right[len2];
   for (int i = 0; i < len1; i++)
      left[i] = arr[l + i]; 
   for (int i = 0; i < len2; i++)
      right[i] = arr[m + 1 + i]; 
   int i = 0;
   int j = 0;
   int k = l;
   while (i < len1 && j < len2) {
      if (left[i] <= right[j]) { //IF element in left is smaller
         arr[k] = left[i];
         i++;
      } else {
         arr[k] = right[j]; //element in right array is greater
         j++;
      }
      k++;
   }
   while (i < len1) { //Copy remaining elements in left array
      arr[k] = left[i];
      k++;
      i++;
   }
   while (j < len2) {//Copy remaining elements in right array
      arr[k] = right[j];
      k++;
      j++;
   }
}
void timSort(int arr[], int n){

   for (int i = 0; i < n; i+=RUN)
       insertionSort( arr, i, min( (i+31), (n-1) ) ); 

   cout << "After insertion sort-" << endl;
   printArray(arr, n);

   for (int size = RUN; size < n; size = 2*size) {//Start Merging from size RUN
      for (int left = 0; left < n;left += 2*size){
         int mid = left + size - 1; //Find end point of left sub array

         int right = min((left + 2*size - 1), (n-1));

         if (mid < right) {
         	merge(arr, left, mid, right); //merge sub array
               }
      }
   }
}

void printArray(int arr[], int n){
   for (int i = 0; i < n; i++)
      cout << arr[i] << " ";
   cout << endl;
}
