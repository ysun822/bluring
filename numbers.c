#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
float str2[101];
int indexString=0;
int quit=0;
pthread_mutex_t lock;
//continue the loop
void *continue_loop(void*a){
    char str[101];
    float num;
    while(quit!=1){
        fgets(str,100,stdin);
        //stop the thread
        if(strcmp(str,"q\n")==0){
            pthread_mutex_lock(&lock);
            quit=1;
            pthread_mutex_unlock(&lock);
            break;
        }
        num=atof(str);
        //ignore the non-num content
      if(num==0.0&&str[0]!=48){
          continue;
      }else {
          pthread_mutex_lock(&lock);
          str2[indexString]=num;
          indexString++;
          pthread_mutex_unlock(&lock);
      }
      
    }
   return 0;
}

//display the info
void* pause_display(void*b){
    //initialize min max
    float min=INT_MAX;
    float max=INT_MIN;
    float average=0;
    float sum=0;
    float cur_index=0;
    float lastFive[5];
    int localquit=0;
    int localindexString=0;
      pthread_mutex_lock(&lock);
       localquit=quit;
       pthread_mutex_unlock(&lock);
    while(localquit!=1){
    sleep(10);
        pthread_mutex_lock(&lock);
        //calculate the min max and average, record the last five number
       localindexString=indexString;
        pthread_mutex_unlock(&lock);
    for(int i=cur_index;i<localindexString;i++){
        if(str2[i]>max){
            max=str2[i];
        }
        if(str2[i]<min){
            min=str2[i];
        }
        sum+=str2[i];
       
    }
        cur_index=localindexString;
        average=sum/localindexString;
        
        int count=0;
       if(localindexString>=5){
           for(int k=localindexString-1;k>localindexString-6;k--){
               lastFive[count]=str2[k];
               count++;
           }
       }else{
        for(int k=localindexString-1;k>=0;k--){
            lastFive[count]=str2[k];
            count++;
        }
    }
        
        //print the info
     printf("\nThe maximum value is %f\n",max);
     printf("The minimum value is %f\n",min);
     printf("The average value is %f\n",average);
      printf("The lastfive value is: ");
        for(int j=0;j<count;j++){
         printf("%f ",lastFive[j] );
}
       pthread_mutex_lock(&lock);
       localquit=quit;
        pthread_mutex_unlock(&lock);
    }
  void* v = NULL;
  pthread_exit(v);
}

int main(){
    pthread_t thread1, thread2;
    void*rt1;
    void *rt2;
    float a=3;
    float b=3;
    //create and join the thread
    pthread_create(&thread1,NULL,&continue_loop,&a);
    pthread_create(&thread2,NULL,&pause_display,&b);
    pthread_join(thread1,&rt1);
    pthread_join(thread2,&rt2);
    return 0;
}