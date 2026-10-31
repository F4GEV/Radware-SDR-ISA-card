#ifndef ___FFTS_DEFINED
#define ___FFTS_DEFINED

int	fft1024(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft512(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft256(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft128(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft64(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft32(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft16(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int	fft8(int rl_in[], int im_in[], int rl_out[], int im_out[]);

int     ifft1024(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft512(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft256(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft128(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft64(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft32(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft16(int rl_in[], int im_in[], int rl_out[], int im_out[]);
int     ifft8(int rl_in[], int im_in[], int rl_out[], int im_out[]);

#endif


