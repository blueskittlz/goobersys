extern "C" void isr_handler() {
    // For now, just hang on any interrupt
    while (true) {
        asm volatile("hlt");
    }
}
