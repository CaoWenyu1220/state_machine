#include <stdio.h>
#include <assert.h>
#include "state_inherit.h"

static QState srcState;	// source state
static QState actState;	// active state

static QEVENT const pkgStdEvt[] = {
	{Q_EMPTY_SIG, 0, 0},
	{Q_INIT_SIG, 0, 0},
	{Q_ENTRY_SIG, 0, 0},	
	{Q_EXIT_SIG, 0, 0}
};

void Q_Initial(QEVENT const *pQevt){
	printf("Top_Init;");	
}	

void Q_Dispatch(QEVENT const *pQevt) {
	for (srcState = actState; srcState; srcState = (QState)(*srcState)(pQevt)){}	
}

void Init_(QState target){
	actState = target;	
}

void Q_Init(QSTATE target){
	register QState s;
	actState = (QState)target;
	srcState = (QState)Q_Initial;
	s = actState; 				// save actState in a temporary
	(*(QPseudoState)srcState)((QEVENT*)0); // top-most initial tran.
	// initial transition must go one level deep
	s = actState; 				// update the temporary
	Q_TRIGGER(s, Q_ENTRY_SIG); 	// enter the state
	while (0 == Q_TRIGGER(s, Q_INIT_SIG)) {
		// init handled
		// initial transition must go one level deep
		s = actState;
		Q_TRIGGER(s, Q_ENTRY_SIG); // enter the substate
	}	
}

void Q_TranDyc(QState target) {
	QState entry[8], p, q, s, *e, *lca;
	for (s = actState; s != srcState; ) {
		QState t;
		t = Q_TRIGGER(s, Q_EXIT_SIG);
		if (t) {	// exit action unhandled, t points to superstate
			s = t;
		} else {	// exit action handled, elicit superstate
			s = Q_TRIGGER(s, Q_EMPTY_SIG);
		}
	}
	*(e = &entry[0]) = 0;
	*(++e) = target; // assume entry to target
	// (a) check source == target (transition to self)
	if (srcState == target) {
		Q_TRIGGER(srcState, Q_EXIT_SIG); // exit source
		goto inLCA;
	}
	// (b) check source == target->super
	p = Q_TRIGGER(target, Q_EMPTY_SIG);
	if (srcState == p) goto inLCA;
	//(c) check source->super == target->super (most common)
	q = Q_TRIGGER(srcState, Q_EMPTY_SIG);
	if (q == p) {
		Q_TRIGGER(srcState, Q_EXIT_SIG); // exit source
		goto inLCA;
	}
	// (d) check source->super == target
	if (q == target) {
		Q_TRIGGER(srcState, Q_EXIT_SIG); // exit source
		--e; // not enter the LCA
		goto inLCA;
	}
	// (e) check rest of source == target->super->super... hierarchy
	*(++e) = p;
	for (s = Q_TRIGGER(p, Q_EMPTY_SIG); s; s = Q_TRIGGER(s, Q_EMPTY_SIG)) {
		if (srcState == s) {
			goto inLCA;
		}
		*(++e) = s;
	}
	Q_TRIGGER(srcState, Q_EXIT_SIG); // exit source
	// (f) check rest of source->super == target->super->super...
	for (lca = e; *lca; --lca) {
		if (q == *lca) {
			e = lca - 1; // do not enter the LCA
			goto inLCA;
		}
	}
	// (g) check each srcState->super->super..for each target...
	for (s = q; s; s = Q_TRIGGER(s, Q_EMPTY_SIG)) {
		for (lca = e; *lca; --lca) {
			if (s == *lca) {
				e = lca - 1; // do not enter the LCA
				goto inLCA;
			}
		}
		Q_TRIGGER(s, Q_EXIT_SIG); // exit s
	}
	assert(0); 	// malformed HSM
inLCA: 			// now we are in the LCA of srcState and target
	assert(e <&entry[ sizeof (entry) / sizeof (*entry)]); // entry fits
	while (s = *e--) {
		// retrace the entry path in reverse order
		Q_TRIGGER(s, Q_ENTRY_SIG); // enter s
	}
	actState = target; 	// update current state
	while (0 == Q_TRIGGER(target, Q_INIT_SIG)) {
		// initial transition must go one level deep
		assert(target == Q_TRIGGER(actState, Q_EMPTY_SIG));
		target = actState;
		Q_TRIGGER(target, Q_ENTRY_SIG); // enter target
	}
}


