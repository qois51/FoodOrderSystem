# Process Development
Develop di branch lain terlebih dahulu, lalu lakukan merge request ke branch main

## Build
1. Jika belum ada folder ``build``, buat terlebih dahulu
    ```
    mkdir -p build
    ```
2. ``cd`` ke folder build
    ```
    cd build
    ```
3. Jalankan ``cmake``
    ```
    cmake -G "MinGW Makefiles" ..
    ```

## Development
Pastikan sudah ada ``cd build`` folder, lalu jalankan :
```
cmake --build .
./main.exe
```

atau
```
make run
```