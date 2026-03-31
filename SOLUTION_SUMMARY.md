# Solution Summary - Problem 090 (ACMOJ 2208)

## Final Score: 100/100 ✓

## Submission History
1. **Submission 766668** (Attempt 1): 50/100 - Runtime Error in get_equivalent_resistance
2. **Submission 766671** (Attempt 2): **100/100** - All tests passed ✓

## Problem Description
Implement a resistive network calculator with three main functions:
1. Calculate equivalent resistance between two nodes
2. Calculate voltage at a node given current distribution
3. Calculate power in network given voltage distribution

## Implementation Approach

### Matrix Class
- Implemented full matrix operations: constructors, copy, move, operators
- Matrix multiplication for computing M = A^T * C * A
- Transpose operation
- **Gaussian elimination** for determinant calculation with:
  - Pivot selection to avoid division by zero
  - Row swapping with sign tracking
  - Special handling for 0x0 and 1x1 matrices

### Resistive Network Class

#### Constructor
- Built incidence matrix **A** (m×n): represents connection topology
  - A[i][j] = 1 if j = from(i)
  - A[i][j] = -1 if j = to(i)
  - A[i][j] = 0 otherwise
- Built conductance matrix **C** (m×m): diagonal matrix with 1/r values

#### get_equivalent_resistance
- Formula: R_ij = det(M_ij) / det(M_i)
- M = A^T * C * A (conductance matrix of network)
- M_i: M with row i and column i removed
- M_ij: M with rows i,j and columns i,j removed
- **Key fix**: Handle special case where i == j (return 0)

#### get_voltage
- Formula: u_i = det(M_i^I) / det(M_n) (Cramer's rule)
- M_n: M with last row and column removed
- M_i^I: M with column i replaced by current vector I, then last row/column removed

#### get_power
- Formula: P = Σ(u_w[i]^2 / r[i])
- u_w = A * U^T (voltage across each wire)
- Calculate power dissipation in each resistor and sum

## Key Bug Fixes (Submission 2)
1. **Special case handling**: Return 0 for same node resistance
2. **Matrix constructor**: Allow 0x0 matrices (for edge cases)
3. **Determinant edge cases**: Handle 1x1 and empty matrices properly

## Test Results
All 20 test points passed:
- Tests 1-3 (get_power): ✓ Accepted
- Test 4 (get_equivalent_resistance, small): ✓ Accepted
- Tests 5-6 (get_equivalent_resistance): ✓ Accepted
- Tests 7-8 (get_voltage): ✓ Accepted
- Tests 9-10 (all functions): ✓ Accepted
- Tests 11-20 (with memory leak detection): ✓ Accepted

## Performance
- Max memory: ~50 MB
- Max time: ~714 ms
- No memory leaks detected

## Code Quality
- Clean implementation with proper memory management
- Comprehensive error handling
- Efficient Gaussian elimination algorithm
- Well-documented code structure
