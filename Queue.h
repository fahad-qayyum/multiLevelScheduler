
/*	Name : Fahad Qayyum
 * 	EECS login: fahadq
 * 	YU ID : 215287253
 * 	Assignment # 2 Question 2
 * 	Course Code : EECS 3221
 * 	Course Name : Operating systems*/
 
typedef struct{   // Structure to store the returned PID and TIME
	int pid;
	int time;
}Task;


typedef struct node{    // Node would store the Task and the next node address
	Task *Task;
	struct node * next;
}Node;

typedef struct{    // Queues would have the head and tail
	Node *head;
	Node *tail;
}Queue;

int sizeOf(Queue *queue)  // returns the size of the QUEUE
{
  int size = 0;
  if(queue->head == NULL){
    return 0;
  }
  else{
    Node *node = queue->head;
    while(node != NULL){
		  size=size + 1;
      node = node->next;	
	  }	  
    return size; 
  }
}

Task *makeTask(int pid, int time)    // creating Task storing pid and time   
{
	Task * task;
  task = (Task*)malloc(sizeof(Task));
	task->pid = pid;
	task->time = time;
	return task;
}

Node *makeNode(Task *Task)    // Defining the Node structure 
{
	Node * node;
  node = (Node*)malloc(sizeof(Node));
	node->Task = Task;
	node->next = NULL;
	return node;
}

Queue *makeQueue()    // Defining the Queue structure
{
	Queue * queue;
  queue = (Queue*)malloc(sizeof(Queue));
	queue->tail = NULL;
	queue->head = NULL;
	return queue;
}

void enqueue(Queue *queue, Task *Task) // adding tasks to the queues
{
	if(sizeOf(queue) == 0)
	{
		queue->head = makeNode(Task);
		queue->tail = queue->head;	
	}	
	else
	{
		queue->tail->next = makeNode(Task);
		queue->tail = queue->tail->next;	
	}
}

Task *dequeue(Queue *queue)  // dequeuing from the Task based on FIFO
{
  if(sizeOf(queue) == 0)
	{
		return NULL;	
	}	
	else
	{
		Node *node = queue->head;
		Task *Task = node->Task;
		queue->head = node->next;
		free(node);
		return Task;	
	}
}


