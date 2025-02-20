__kernel void life(__global int **board, __global int **next) {
    uint rowid = get_global_id(0);
    uint colid = get_global_id(1);
    uint maxrow = get_global_size(0);
    uint maxcol = get_global_size(1);

    uint is_alive = board[rowid][colid];
    uint neighbors = 0;
    for(int i = rowid - 1; i <= rowid + 1; i++) {
        if(i < 0 || i >= maxrow)
            continue;
        for(int j = colid - 1; j <= colid + 1; j++) {
            if(j < 0 || j >= maxcol || (i == rowid && j == colid))
                continue;
            neighbors += board[i][j];
        }
    }

    if(neighbors < 2)
        next[rowid][colid] = 0;
    if(neighbors > 3)
        next[rowid][colid] = 0;
    if(is_alive == 0 && neighbors == 3)
        next[rowid][colid] = 1;
}