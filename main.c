#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define MAX_INPUT 9999
#define APP_NAME L"SorteringsalgoritmerGUI"
#define ID_INSERTIONSORT 1
#define ID_BUBBLESORT 2
#define ID_MERGESORT 3
#define ID_INPUTFIELD 4
#define ID_OUTPUTFIELD 5
#define ID_RANDOMINPUT 6
#define ID_QUIT 7

static HWND inputField, outputField;
static HWND error, timeElapsed;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASSW wc = { 0 };
    wc.lpszClassName = APP_NAME;
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, APP_NAME,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        150, 150, 640, 360, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    HDC hdc;
    PAINTSTRUCT ps;

    DWORD color;

    HFONT hFont, holdFont;

    switch (msg) {

    case WM_CREATE:
        WMCreate(hwnd);
        break;

    case WM_COMMAND:

        if (LOWORD(wParam) == ID_INSERTIONSORT) {
            sort(1);
        }

        if (LOWORD(wParam) == ID_BUBBLESORT) {
            sort(2);
        }

        if (LOWORD(wParam) == ID_MERGESORT) {
            sort(3);
        }

        if (LOWORD(wParam) == ID_RANDOMINPUT) {
            randomInput();
        }

        if (LOWORD(wParam) == ID_QUIT) {
            PostQuitMessage(0);
        }

        break;

    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}


int WMCreate(HWND hwnd) {
    CreateWindowW(L"STATIC", L"Input",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        20, 20, 125, 20, hwnd, (HMENU)1000,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

    inputField = CreateWindowA("EDIT", NULL,
        ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_VISIBLE | WS_CHILD,
        20, 40, 150, 260, hwnd, (HMENU)ID_INPUTFIELD, NULL, NULL);

    CreateWindowA("Button", "Insertion Sort",
        WS_VISIBLE | WS_CHILD,
        220, 50, 150, 25, hwnd, (HMENU)ID_INSERTIONSORT, NULL, NULL);

    CreateWindowA("Button", "Bubble Sort",
        WS_VISIBLE | WS_CHILD,
        220, 80, 150, 25, hwnd, (HMENU)ID_BUBBLESORT, NULL, NULL);

    CreateWindowA("Button", "Merge Sort",
        WS_VISIBLE | WS_CHILD,
        220, 110, 150, 25, hwnd, (HMENU)ID_MERGESORT, NULL, NULL);

    CreateWindowA("Button", "Randomize",
        WS_VISIBLE | WS_CHILD,
        220, 140, 150, 25, hwnd, (HMENU)ID_RANDOMINPUT, NULL, NULL);

    CreateWindowA("Button", "Quit",
        WS_VISIBLE | WS_CHILD,
        220, 170, 150, 25, hwnd, (HMENU)ID_QUIT, NULL, NULL);

    CreateWindowW(L"STATIC", L"Output",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        450, 20, 125, 20, hwnd, (HMENU)1000,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

    outputField = CreateWindowA("EDIT", NULL,
        ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_VISIBLE | WS_CHILD,
        450, 40, 150, 260, hwnd, (HMENU)ID_OUTPUTFIELD, NULL, NULL);

    error = CreateWindowA("STATIC", "Status: ",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        220, 200, 125, 20, hwnd, (HMENU)1000,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

    timeElapsed = CreateWindowA("STATIC", "Time Elapsed: ",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        220, 220, 200, 20, hwnd, (HMENU)1000,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
}

int sort(int n) {
    timer();
    char* charInput = malloc(GetWindowTextLengthA(inputField) + 1);
    GetWindowTextA(inputField, charInput, GetWindowTextLengthA(inputField)+1);
    int length = 1;
    for (int i = 0; i < GetWindowTextLengthA(inputField) + 1; i++) {
        if (charInput[i] == '\n' || charInput[i] == '\0') {
            length++;
        }
    }
    int input[MAX_INPUT];
    length = 0;
    char buffer[64];
    int ind = 0;
    for (int i = 0; i < GetWindowTextLengthA(inputField)+1; i++) {
        if (charInput[i] == '\n' || charInput[i] == '\0') {
            input[length] = atoi(buffer);
            for (int j = 0; j < ind + 2; j++)
                buffer[j] = NULL;
            length++;
            ind = 0;
        }
        else {
            buffer[ind] = charInput[i];
            ind++;
        }
    }
    switch (n) {
    case 1:
        insertionSort(length, input);
        break;
    case 2:
        bubbleSort(length, input);
        break;
    case 3:
        mergeSort(length, input);
        break;
    }
    free(charInput);
    return 0;
}

// Insertion Sort method
int insertionSort(int length, int input[]) {
    for (int i = 1; i < length; i++) {
        // Copy 'input[i]' so it does not get overwritten
        int j, temp = input[i];
        // Go up through 'input[]' starting at 'input[1]' and check if the integer before it in the array is bigger
        for (j = i - 1; j >= 0 && input[j] > temp; j--) {
            // If the integer before it is bigger, move it up one step then repeat but for the next integer, 
            // until the next one is smaller or there are none remaining
            input[j + 1] = input[j];
        }
        // Put the temp integer into its sorted place
        input[j + 1] = temp;
    }
    verifySorting(length, input);
    return 0;
}

// Bubble Sort method
int bubbleSort(int length, int input[]) {
    int swapped;
    for (int i = 0; i < length - 1; i++) {
        // Reset swapped for this loop
        swapped = 0;
        for (int j = 0; j < length - i - 1; j++) {
            // Go through the first and second integer and swap them if the first one is bigger, then repeat
            // with the second and third, and so on until it reaches the end minus the amount of loops before this one
            if (input[j] > input[j + 1]) {
                int temp = input[j];
                input[j] = input[j + 1];
                input[j + 1] = temp;
                // Set 'swapped' to 1 (one or more values were swapped during this loop)
                swapped = 1;
            }
        }
        // If no values were swapped during last loop, exit early
        if (swapped == 0) {
            break;
        }
    }
    verifySorting(length, input);
    return 0;
}

// Merge Sort method
int arr[MAX_INPUT][MAX_INPUT];
int arrLen[MAX_INPUT];
int mergeSort(int length, int input[]) {
    // Split the input into different arrays each containing one integer each
    for (int i = 0; i < length; i++) {
        arr[i][0] = input[i];
        arrLen[i] = 1;
    }
    // Make a loop that goes through all pairs of arrays, then merges them, then goes through the new arrays in pairs and merges them until everything is sorted
    int j = 1;
    for (int width = 1; width < length; width *= 2) {
        for (int i = 0; i < length; i = i + 2 * width) {
            // Send both elements that will get merged to the merge function
            merge(i, i + j, length, input);
        }
        j *= 2;
    }
    // Once the merge process is done, copy the end result to the input array
    for (int i = 0; i < length; i++) {
        input[i] = arr[0][i];
    }
    verifySorting(length, input);
    return 0;
}

int merge(int a, int b, int length, int input[]) {
    // If the second index is out of range, the first one should already be sorted, return then
    if (b >= length)
        return 0;
    // Make a temp array to put the newly merged integers to
    int temp[MAX_INPUT];
    int i = 0;
    int j = 0;
    int k = 0;
    // While there are still integers left in both arrays, compare them and add the smallest until nothing is left of one array
    while (i < arrLen[a] && j < arrLen[b]) {
        if (arr[a][i] < arr[b][j]) {
            temp[k] = arr[a][i];
            i++;
        }
        else {
            temp[k] = arr[b][j];
            j++;
        }
        k++;
    }
    // If there is something left in the first array, add it to the end of the temp array
    while (i < arrLen[a]) {
        temp[k] = arr[a][i];
        i++;
        k++;
    }
    // If there is something left in the second array, add it to the end of the temp array
    while (j < arrLen[b]) {
        temp[k] = arr[b][j];
        j++;
        k++;
    }
    // Copy the temp array to the first array
    for (int l = 0; l < (arrLen[a] + arrLen[b]); l++) {
        arr[a][l] = temp[l];
    }
    // Save the new length of the first array
    arrLen[a] = k;
    return 0;
}

// Verify Sorting
int verifySorting(int length, int input[]) {
    system("cls");
    // Go through every number in the array
    for (int i = 1; i < length; i++) {
        // If the array is not sorted, say an error occurred
        if (input[i - 1] > input[i]) {
            SetWindowTextA(error, "Status: An error occurred.");
            return 0;
        }
    }
    //printf("\033[32mList was successfully sorted!\n");
    SetWindowTextA(error, "Status: Success!");
    displayList(input, length);
    return 0;
}

// Display List
int displayList(int input[], int length) {
    int max = sizeof("\r\n%d") * (length - 1) + sizeof("%d") + 1;
    char* display = malloc(max);
    char buffer[64];
    snprintf(buffer, 64, "%d", input[0]);
    strcpy_s(display, max, buffer);
    for (int i = 1; i < length; i++) {
        snprintf(buffer, 64, "\r\n%d", input[i]);
        strcat_s(display, max, buffer);
    }
    SetWindowTextA(outputField, display);
    free(display);
    timer();
    return 0;
}

// Timer Logic
int time = 0;
int timer() {
    // Set value of Time on every uneven call
    if (time == 0) {
        time = GetTickCount64();
    }
    // Print the time since last call on every even call
    else {
        char display[200];
        snprintf(display, 200, "Time elapsed: %dms", (int)GetTickCount64() - time);
        SetWindowTextA(timeElapsed, display);
        time = 0;
    }
    return 0;
}

int randomInput() {

}