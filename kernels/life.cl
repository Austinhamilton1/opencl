__kernel void life(__global int *board, __global int *next) {
    int rowid = get_global_id(0);
    int colid = get_global_id(1);
    uint maxrow = get_global_size(0);
    uint maxcol = get_global_size(1);
    uint idx = maxcol * rowid + colid;

    uint is_alive = board[idx];
    uint neighbors = 0;
    for(int i = rowid - 1; i <= rowid + 1; i++) {
        if(i < 0 || i > maxrow-1)
            continue;
        for(int j = colid - 1; j <= colid + 1; j++) {
            if(j < 0 || j > maxcol-1)
                continue;
            if(i == rowid && j == colid)
                continue;
            neighbors += board[maxcol * i + j];
        }
    }

    next[idx] = is_alive;
    if(neighbors < 2)
        next[idx] = 0;
    if(neighbors > 3)
        next[idx] = 0;
    if(is_alive == 0 && neighbors == 3)
        next[idx] = 1;
}