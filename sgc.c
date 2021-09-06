#include "sgc.h"

static VM* initVM(void){
	VM* vm = (VM*)malloc(sizeof(VM));
	vm->stackSize = 0;
	vm->numObjects = 0;
	vm->maxObjects = MAX_GC_THRESHOLD;
	return vm;
};
/*  */
static void pushToVM(VM* vm, Object* value){
	assert(vm->stackSize < STACK_MAX, "Stack OverFlow!");
	vm->stack[vm->stackSize++] = value;
};
/*  */
static Object* popFromVM(VM* vm){
	assert(vm->stackSize > 0, "Stack UnderFlow!");
	return vm->stack[--vm->stackSize];
};
/*  */
static Object* newObject(VM* vm, ObjectType type){
	if (vm->numObjects >= vm->maxObjects)
		gc(vm); 

	Object* object = (Object*)malloc(sizeof(Object));
	object->type = type;
	object->marked = 0;

	object->next = vm->firstObject;
	vm->firstObject = object;

	vm->numObjects++;
	return object;
};
/*  */
static void pushINT(VM* vm, int value){
	Object* object = newObject(vm, OBJ_INT);
	object->value = value;
	pushToVM(vm, object);
};
/*  */
static Object* pushPAIR(VM* vm){
	Object* object = newObject(vm, OBJ_PAIR);
	object->tail = popFromVM(vm);
	object->head = popFromVM(vm);

	pushToVM(vm, object);
	return object;
};
/*  */
static void mark(Object* object){
	if (object->marked)								// breakpoint, if we reach already marked object
		return; 

	object->marked = 1;

	if (object->type == OBJ_PAIR) {					// defined that if obj is pair, then recursive mark	
		mark(object->tail);							// two his reachable fields
		mark(object->head)
	}
};

static void markALL(VM* vm){
	for (size_t i = 0; i < vm->stackSize - 1; i++) {
		mark(vm->stack[i]);
	}
};

static void sweep(VM* vm){
	Object** object = &vm->firstObject;

	while (*object) {
		if (!(*object)->marked) {					// object is unreached, then remove
			Object* unreached = *object;
			*object = unreached->next;
			vm->numObjects--;
			free(unreached);
		} else {
			(*object)->marked = 0;
			object = &(*object)->next;
		}	
	}
};

static void gc(VM* vm){
	int numObjects = vm->numObjects;

	markALL(vm);
	sweep(vm);

	vm->maxObject = vm->numObjects * 2;
};