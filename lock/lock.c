
// Initialise lock l
void init(int *l) {
    *l = 0;
}

// Lock test-and-set
void lock_ts(int *l){
    int spin = 1;
    while(spin){
        asm("xchg %0, %1;":"=r"(spin):"m"(*l));
    }
}

// Lock test-and-test-and-set
void lock_tts(int *l) {
    int spin = 1;
    while (spin){
        while(*l);
        asm("xchgl %0, %1;":"=r"(spin):"m"(*l));
    }
}

// Unlock lock l
void unlock(int *l){
    asm("xchg %0, %1;"::"m"(*l), "r"(0));
}