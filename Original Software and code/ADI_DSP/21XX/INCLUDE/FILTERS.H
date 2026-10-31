int	biquad(int sample, int pm coeffs[], int delay[], int sections);
int	fir(int sample, int pm coeffs[], int delay[], int taps);
int	iir(int sample, int pm a_coeffs[], int pm b_coeffs[], int delay[], int taps);

int     demean_buffer(int input[], int oldmean, int LENGTH);

