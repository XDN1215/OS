#include "lib.h"
#include "types.h"
#define N 5
#define bufN 10

void philosopher(int i, pid_t pid, sem_t *mutex, sem_t forks[]);
void producer(pid_t pid, sem_t *empty, sem_t *mutexp, int product);
void consumer(pid_t pid, sem_t *full, sem_t *mutexc, int product);

int uEntry(void) {

	// 测试scanf	
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while(1){
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf(" Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}
	
	// 测试信号量
	int i = 4;
	sem_t sem;
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 0);
	if (ret == -1) {
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0) {
		while( i != 0) {
			i --;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i --;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}

	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	
	//哲学家
	/*sem_t forks[N], mutex;
	for(int i = 0;i < N;i++){
		sem_init(&forks[i], 1);
	}
	sem_init(&mutex, N-1);
	int ret[4];
	for(int i = 0;i < 4;i++){
		ret[i] = -1;
	}
	for(int i = 0;i < 4;i++){
		if(i == 0 || (i > 0 && ret[i - 1] == 0))
			ret[i] = fork();
	}	
	for(int i = 0;i < 4;i++){
		if(ret[i] == 0){
			pid_t pid = getpid();
			philosopher(pid - 1, pid, &mutex, forks);
		}
		else if(i == 0 && ret[i] == 2){
			pid_t pid = getpid();
			philosopher(pid - 1, pid, &mutex, forks);
		}
	}

	//生产者消费者问题
	sem_t empty;sem_init(&empty, bufN);
	sem_t full;sem_init(&full, 0);
	sem_t mutexp;sem_init(&mutexp, 1);
	sem_t mutexc;sem_init(&mutexc, 1);
	int ret[4];
	for(int i = 0;i < 4;i++){
		ret[i] = -1;
	}
	for(int i = 0;i < 4;i++){
		if(i == 0 || (i > 0 && ret[i - 1] == 0))
			ret[i] = fork();
	}
	int product = -1;
	for(int i = 0;i < 4;i++){
		if(i == 0 && ret[i] == 2){
			pid_t pid = getpid();
			consumer(pid, &full, &mutexc, product);
		}
		else if(ret[i] == 0){
			pid_t pid = getpid();
			producer(pid, &empty, &mutexp, product);
		}
	}*/

	//读者写者问题
	

	exit(0);
	return 0;
}

void philosopher(int i, pid_t pid, sem_t *mutex, sem_t forks[]){ // 哲学家编号：0-4
  while(1){
    printf("Philosopher %d: think\n", pid); // 哲学家在思考
    sleep(128);
	sem_wait(mutex);              // 进入临界区
    sleep(128);
	sem_wait(&forks[i]);            // 去拿左边的叉子
    sleep(128);
	sem_wait(&forks[(i+1)%N]);      // 去拿右边的叉子
    sleep(128);
	printf("Philosopher %d: eat\n", pid); // 吃面条
    sleep(128);
	sem_post(&forks[i]);            // 放下左边的叉子
    sleep(128);
	sem_post(&forks[(i+1)%N]);      // 放下右边的叉子
    sleep(128);
	sem_post(mutex);              // 退出临界区
	sleep(128);
  }
}

void producer(pid_t pid, sem_t *empty, sem_t *mutexp, int product){
	while(1){
		sem_wait(empty);
		sleep(128);
		sem_wait(mutexp);
		sleep(128);
		product = append_to_buffer();
		printf("Producer %d: produce product %d\n", pid, product);
		sleep(128);
		sem_post(mutexp);
		sleep(128);
		sem_post(empty);
		sleep(128);
	}
}

void consumer(pid_t pid, sem_t *full, sem_t *mutexc, int product){
	while(1){
		sem_wait(full);
		sleep(128);
		sem_wait(mutexc);
		sleep(128);
		product = take_from_buffer();
		printf("Consumer %d: consume product %d\n", pid, product);
		sleep(128);
		sem_post(mutexc);
		sleep(128);
		sem_post(full);
		sleep(128);
	}
}