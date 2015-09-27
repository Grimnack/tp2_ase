#include <stdlib.h>
#include <stdio.h>

typedef void (func_t) (void *) ;

enum ctx_state_e { INITIALIZED, ACTIVABLE, TERMINATED} ;

struct ctx_s {
   unsigned char *ctx_stack;
	void* ctx_ebp ;
	void* ctx_esp ;
	int magic ;
	void* ctx_arg ;
	func_t* f ;
	enum ctx_state_e ctx_state;
} ;

int init_ctx (struct ctx_s* ctx, int stack_size, func_t f, void* args){
   ctx->ctx_stack = (unsigned char*) malloc (stack_size);
	ctx->ctx_ebp = ctx->ctx_stack[stack_size - sizeof(void *)];
	ctx->ctx_esp = ctx->ctx_stack[stack_size - sizeof(void *)];
	ctx->magic = 0xB00B5666 ;
	ctx->f = f;
	ctx->ctx_arg = args ;
	ctx-> ctx_state = INITIALIZED;
	return 0;
}

struct ctx_s *ctx_courant =NULL ;

/*

*/
void switch_to_ctx (struct ctx_s* ctx){
	/*Premiere etape, sort-on d'un contexte 
   si oui on sauvegarde le contexte qu'on a quitté*/
	if(ctx_courant!=NULL){
		asm("movl %%ebp, %0 \n\t movl %%esp, %1"
			:"=r" (ctx_courant->ctx_ebp),
			"=r" (ctx_courant->ctx_esp));
	}else{
		ctx_courant = ctx;
	}
	/*Deuxieme etape, on change de contexte*/
	asm("movl %0, %%ebp \n\t movl %1, %%esp"
		::"r" (ctx->ctx_ebp),
		"r" (ctx->ctx_esp));
	if(ctx_courant->ctx_state==INITIALIZED){
      ctx_courant->ctx_state=ACTIVABLE;
      (ctx_courant->f) (ctx_courant->ctx_arg);
		return ;
	};
	return ;

}

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_pong(void*);
void f_ping(void*);


int main (void){
	init_ctx(&ctx_ping,16384, f_ping, NULL);
	init_ctx(&ctx_pong,16384, f_pong, NULL);
	switch_to_ctx(&ctx_ping);

	exit(EXIT_SUCCESS);
}


void f_ping(void* args){
	while(1){
		printf("A");
		switch_to_ctx(&ctx_pong);
		printf("B");
		switch_to_ctx(&ctx_pong);
		printf("C");
		switch_to_ctx(&ctx_pong);
	}
}

void f_pong(void* args){
	while(1){
		printf("1");
		switch_to_ctx(&ctx_ping);
		printf("2");
		switch_to_ctx(&ctx_ping);
	}
}







