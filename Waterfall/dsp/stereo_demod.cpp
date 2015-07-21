/* -*- c++ -*- */
/*
 * Copyright 2012 Alexandru Csete OZ9AEC.
 * FM stereo implementation by Alex Grinkov a.grinkov(at)gmail.com.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <gr_io_signature.h>
#include <math.h>
#include <iostream>
#include <dsp/stereo_demod.h>


/* Create a new instance of stereo_demod and return a boost shared_ptr. */
stereo_demod_sptr make_stereo_demod(float quad_rate, float audio_rate,
                                    bool stereo)
{
    return gnuradio::get_initial_sptr(new stereo_demod(quad_rate,
                                                       audio_rate, stereo));
}


static const int MIN_IN  = 1; /* Mininum number of input streams. */
static const int MAX_IN  = 1; /* Maximum number of input streams. */
static const int MIN_OUT = 2; /* Minimum number of output streams. */
static const int MAX_OUT = 2; /* Maximum number of output streams. */

#define STEREO_DEMOD_PARANOIC

/*! \brief Create stereo demodulator object.
 *
 * Use make_stereo_demod() instead.
 */
stereo_demod::stereo_demod(float input_rate, float audio_rate, bool stereo)
    : gr_hier_block2("stereo_demod",
                     gr_make_io_signature (MIN_IN,  MAX_IN,  sizeof (float)),
                     gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (float))),
    d_input_rate(input_rate),
    d_audio_rate(audio_rate),
    d_stereo(stereo)
{
  lpf0 = make_lpf_ff(d_input_rate, 17e3, 2e3); // FIXME
  audio_rr0 = make_resampler_ff(d_audio_rate/d_input_rate);

  if (d_stereo)
  {
    lpf1 = make_lpf_ff(d_input_rate, 17e3, 2e3); // FIXME
    audio_rr1 = make_resampler_ff(d_audio_rate/d_input_rate);

    d_tone_taps = gr_firdes::complex_band_pass(
                                       1.0,          // gain,
		                                   d_input_rate, // sampling_freq
                                       18800.,       // low_cutoff_freq
                                       19200.,       // high_cutoff_freq
                                       300.);        // transition_width
    tone = gr_make_fir_filter_fcc(1, d_tone_taps);

    pll = gr_make_pll_refout_cc(0.001,                        // loop_bw FIXME
                                2*M_PI * 19200 / input_rate,  // max_freq
                                2*M_PI * 18800 / input_rate); // min_freq

    subtone = gr_make_multiply_cc();

    lo = gr_make_complex_to_imag();

#ifdef STEREO_DEMOD_PARANOIC
    d_pll_taps = gr_firdes::band_pass(
                                       1.0,          // gain,
		                                   d_input_rate, // sampling_freq
                                       37600.,       // low_cutoff_freq
                                       38400.,       // high_cutoff_freq
                                       400.);        // transition_width
    lo2 = gr_make_fir_filter_fff(1, d_pll_taps);
#endif

    mixer = gr_make_multiply_ff();

    cdp = gr_make_multiply_const_ff( 2.); // FIXME
    cdm = gr_make_multiply_const_ff(-2.); // FIXME

    add0 = gr_make_add_ff();
    add1 = gr_make_add_ff();

    /* connect block */
    connect(self(), 0, tone, 0);
    connect(tone, 0, pll, 0);
    connect(pll, 0, subtone, 0);
    connect(pll, 0, subtone, 1);
    connect(subtone, 0, lo, 0);

#ifdef STEREO_DEMOD_PARANOIC
    connect(lo,  0, lo2, 0);
    connect(lo2, 0, mixer, 0);
#else
    connect(lo, 0, mixer, 0);
#endif
    connect(self(), 0, mixer, 1);

    connect(self(), 0, lpf0, 0);
    connect(mixer,  0, lpf1, 0);

    connect(lpf0, 0, audio_rr0, 0); // sum
    connect(lpf1, 0, audio_rr1, 0);

    connect(audio_rr1, 0, cdp,  0); // +delta
    connect(audio_rr1, 0, cdm,  0); // -delta

    connect(audio_rr0, 0, add0,   0);
    connect(cdp,       0, add0,   1);
    connect(add0,      0, self(), 0); // left = sum + delta

    connect(audio_rr0, 0, add1,   0);
    connect(cdm,       0, add1,   1);
    connect(add1,      0, self(), 1); // right = sum + delta
  }
  else // if (!d_stereo)
  {
    /* connect block */
    connect(self(), 0, lpf0, 0);
    connect(lpf0,   0, audio_rr0, 0);
    connect(audio_rr0, 0, self(), 0);
    connect(audio_rr0, 0, self(), 1);
  }
}


stereo_demod::~stereo_demod()
{

}

