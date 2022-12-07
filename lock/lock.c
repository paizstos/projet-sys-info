void init(int *l) {
    *l = 0;
}

void lock(int *l){
    int spin = 1;
    while(spin){
        asm("xchg %0, %1;":"=r"(spin):"m"(*l));
    }
}

void unlock(int *l) {
    asm("xchg %0, %1;"::"m"(*l), "r"(0));
}