#pragma once

int abs(int n);
long int labs(long int n);
long long int llabs(long long int n);

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long int quot;
    long int rem;
} ldiv_t;

typedef struct {
    long long int quot;
    long long int rem;
} lldiv_t;

div_t div(int x, int y);
ldiv_t ldiv(long int x, long int y);
lldiv_t lldiv(long long int x, long long int y);


