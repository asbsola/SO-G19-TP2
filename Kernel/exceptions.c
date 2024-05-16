static void zero_division();
static void invalid_op_code();

void (*exceptions[])() = {zero_division, invalid_op_code};

void exceptionDispatcher(int id) {
	exceptions[id]();
}

static void zero_division() {
	// Handler para manejar excepcíon
}

static void invalid_op_code() {
	// Handler para manejar excepcíon
}