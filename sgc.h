#ifndef SGC_H_
#define SGC_H_

/* Num of allocated elements, sufficient to trigger gc */
#define MAX_GC_THRESHOLD 256
/* Max num of elements in simple stack virtual machine */
#define STACK_MAX 256

////////////////////////////////////////////////////////////////////////////
//////////////////////////// Data Defines Block ////////////////////////////
////////////////////////////////////////////////////////////////////////////

/* Examples of simple and composite types */
typedef enum {
	OBJ_INT,
	OBJ_PAIR
} ObjectType;

/* Main types */
typedef struct sObject {
	struct sObject* next;					//  
	unsigned int mark;						// defines if marked sObject or no by gc
	ObjectType type;						// defines what type store a sObject (int or pair)

	union {									// defines what kind of value stored in sObject						
		int value;							// OBJ_INT

		struct {							//OBJ_PAIR
			struct sObject* head;
			struct sObject* tail;
		};
	};
} Object;

/* Simple Stack-based Virtual Machine */
typedef struct {
	Object* firstObject;					// origin of linked list, needed for sweep inmarked objs
	int numObjects;							// counter of objects
	int maxObjects;							// define max objects to triggered gc
	Object* stack[STACK_MAX];
	int stackSize;
} VM;

////////////////////////////////////////////////////////////////////////////
///////////////////////// Data Defines Block End ///////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
///////////////////////// Functions Defines Block //////////////////////////
////////////////////////////////////////////////////////////////////////////

/* Mark:
		Input: Object
		Output: None 
	Mark new object									 */
static void mark(Object* object);

/* MarkALL:
 		Input: Virtual Machine
		Output: None
	Mark all objects, when gc triggered			     */
static void markALL(VM* vm);

/* Sweep:
		Input: Virtual Machine
		Output: None
	"Sweeping" all reachable and not used objects    */
static void sweep(VM* vm);

/* Garbage Collector:
		Input: Virtual Machine
		Output: None
	Remove reachable and not used 
			objects by mark and sweep			     */
static void gc(VM* vm);

/* Init Virtual Machine:
		Input: Virtual Machine
		Output: Virtual Machine
	Initialize virtual machine  					 */
static VM* initVM(void);

/* PushToVM:
		Input: Virtual Machine, Value
		Output: None
	Push object to stack of VM 			 			 */
static void pushToVM(VM* vm, Object* value);

/* PopFromVM:
		Input: Virtual Machine
		Output: Object
	Pop object from stack of VM 					 */
static Object* popFromVM(VM* vm);

/* NewObject:
		Input: Virtual Machine, Object Type
		Output: Object
	Alloc new object and trigger gc, if needed		 */
static Object* newObject(VM* vm, ObjectType type);

/* PushINT:
		Input: Virtual Machine, Value
		Output: None
	Push integer-type object to stack of initVM		 */
static void pushINT(VM* vm, int value);

/* PushPAIR:
		Input: Virtual Machine
		Output: Object
	Push pair-type object to stack of initVM 		 */
static Object* pushPAIR(VM* vm);

////////////////////////////////////////////////////////////////////////////
/////////////////////// Functions Defines Block End ////////////////////////
////////////////////////////////////////////////////////////////////////////

#endif // SGC_H_