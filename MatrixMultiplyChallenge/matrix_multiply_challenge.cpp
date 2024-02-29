/**
 * Challenge: Multiply two matrices
 */
#include <thread>
#include <cmath>

constexpr size_t num_threads = 8; //std::thread::hardware_concurrency();

/* sequential implementation of matrix multiply */
void sequential_matrix_multiply(long ** A, size_t num_rows_a, size_t num_cols_a,
                                long ** B, size_t num_rows_b, size_t num_cols_b,
							    long ** C) {
    for (size_t i=0; i<num_rows_a; i++) {
        for (size_t j=0; j<num_cols_b; j++) {
			C[i][j] = 0; // initialize result cell to zero
            for (size_t k=0; k<num_cols_a; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
		}
	}
}

/* parallel implementation of matrix multiply */
void parallel_matrix_multiply(long ** A, size_t num_rows_a, size_t num_cols_a,
                              long ** B, size_t num_rows_b, size_t num_cols_b,
							  long ** C) {
    /***********************
     * YOUR CODE GOES HERE *
     ***********************/
//    size_t num_threads = 4;
    std::thread threads[num_threads];
    size_t partition = num_rows_a/num_threads;
    size_t start = 0;
    for (size_t i = 0; i < num_threads; i++) 
    {
        threads[i] = std::thread([i, &A, &B, &C, &num_rows_a, &num_cols_a, &num_cols_b, start, partition]()
        {
            for (size_t l=start; l < ((i == num_threads - 1) ? num_rows_a : start + partition); l++) {
                for (size_t j=0; j<num_cols_b; j++) {
                    C[l][j] = 0; // initialize result cell to zero
                    for (size_t k=0; k<num_cols_a; k++) {
                        C[l][j] += A[l][k] * B[k][j];
                    }
                }
            }
        });
        start += partition;
    }
    for (size_t i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }
}

int main() {
	const size_t NUM_EVAL_RUNS = 3;
	const size_t NUM_ROWS_A = 1000;
	const size_t NUM_COLS_A = 1000;
	const size_t NUM_ROWS_B = NUM_COLS_A;
	const size_t NUM_COLS_B = 1000;

    // intialize A with values in range 1 to 100    
    long ** A = (long **)malloc(NUM_ROWS_A * sizeof(long *));
    if (A == NULL) {
        exit(2);
    }
    for (size_t i=0; i<NUM_ROWS_A; i++) {
        A[i] = (long *)malloc(NUM_COLS_A * sizeof(long));
        if (A[i] == NULL) {
            exit(2);
        }
        for (size_t j=0; j<NUM_COLS_A; j++) {
            A[i][j] = rand() % 100 + 1;
        }
    }

    // intialize B with values in range 1 to 100   
    long ** B = (long **)malloc(NUM_ROWS_B * sizeof(long *));
    if (B == NULL) {
        exit(2);
    }
    for (size_t i=0; i<NUM_ROWS_B; i++) {
        B[i] = (long *)malloc(NUM_COLS_B * sizeof(long));
        if (B[i] == NULL) {
            exit(2);
        }
        for (size_t j=0; j<NUM_COLS_B; j++) {
            B[i][j] = rand() % 100 + 1;
        }
    }

	// allocate arrays for sequential and parallel results
    long ** sequential_result = (long **)malloc(NUM_ROWS_A * sizeof(long *));
    long ** parallel_result = (long **)malloc(NUM_ROWS_A * sizeof(long *));
    if ((sequential_result == NULL) || (parallel_result == NULL)) {
        exit(2);
    }
    for (size_t i=0; i<NUM_ROWS_A; i++) {
        sequential_result[i] = (long *)malloc(NUM_COLS_B * sizeof(long));
        parallel_result[i] = (long *)malloc(NUM_COLS_B * sizeof(long));
        if ((sequential_result[i] == NULL) || (parallel_result[i] == NULL)) {
            exit(2);
        }
    }

    printf("Evaluating Sequential Implementation...\n");
	std::chrono::duration<double> sequential_time(0);
    sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result); // "warm up"
    for (size_t i=0; i<NUM_EVAL_RUNS; i++) {
        auto startTime = std::chrono::high_resolution_clock::now();
        sequential_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, sequential_result);
        sequential_time += std::chrono::high_resolution_clock::now() - startTime;
    }
    sequential_time /= NUM_EVAL_RUNS;

    printf("Evaluating Parallel Implementation...\n");
	std::chrono::duration<double> parallel_time(0);
    parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result); // "warm up"
    for (size_t i=0; i<NUM_EVAL_RUNS; i++) {
        auto startTime = std::chrono::high_resolution_clock::now();
        parallel_matrix_multiply(A, NUM_ROWS_A, NUM_COLS_A, B, NUM_ROWS_B, NUM_COLS_B, parallel_result);
        parallel_time += std::chrono::high_resolution_clock::now() - startTime;
    }
    parallel_time /= NUM_EVAL_RUNS;
	
    // verify sequential and parallel results
    for (size_t i=0; i<NUM_ROWS_A; i++) {
        for (size_t j=0; j<NUM_COLS_B; j++) {
            if (sequential_result[i][j] != parallel_result[i][j]) {
                printf("ERROR: Result mismatch at row %ld, col %ld!\n", i, j);
            }
        }
    }
    printf("Average Sequential Time: %.2f ms\n", sequential_time.count()*1000);
    printf("  Average Parallel Time: %.2f ms\n", parallel_time.count()*1000);
    printf("Speedup: %.2f\n", sequential_time/parallel_time);
    printf("Efficiency %.2f%%\n", 100*(sequential_time/parallel_time)/num_threads);
}
