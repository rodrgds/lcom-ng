// Tentative globals from separate units must still merge.
int common_value;
int late1(void);
int read_common_value(void) { return common_value + late1(); }
