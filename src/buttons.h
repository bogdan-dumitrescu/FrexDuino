const uint8_t buttonMatrixRows = 2;
const uint8_t buttonMatrixColumns = 4;

const uint8_t numberOfButtons = buttonMatrixRows * buttonMatrixColumns;

void initBttnMatrixPins();
void readButtons(uint8_t *buttons);