// SPDX-FileCopyrightText: 2006-2025 Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2025 Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

// This example illustrates how we can reduce the usage of templates (or the amount of different instantiations) via
// type erasure. Having only one function generated for `algorithm()` is the only benefit of using `semialphabet_any`
// here. Of course this only makes sense for your application if the part of the program that is agnostic of the
// character representation (your equivalent of `algorithm()`) is substantially larger than the specific parts – and
// if compile-time and/or size of the executable are a concern.

#include <iostream>

#include <seqan3/alphabet/aminoacid/all.hpp>
#include <seqan3/alphabet/composite/semialphabet_any.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/utility/range/to.hpp>

using namespace seqan3::literals;

// Print is a template and gets instantiated two times because the behaviour is different for both types
template <typename rng_t>
void print(rng_t && r)
{
    seqan3::debug_stream << r << '\n';
}

// Algorithm is not a template, only one instance is generated by the compiler
// Type information is encoded via a run-time parameter
void algorithm(std::vector<seqan3::semialphabet_any<10>> & r, bool is_murphy)
{
    // Algorithm example that replaces rank 0 with rank 1
    for (auto & v : r)
        if (seqan3::to_rank(v) == 0)
            seqan3::assign_rank_to(1, v);

    // Here we verify the type for printing
    if (is_murphy)
        print(r
              | std::views::transform(
                  [](auto const & in)
                  {
                      return static_cast<seqan3::aa10murphy>(in);
                  }));
    else
        print(r
              | std::views::transform(
                  [](auto const & in)
                  {
                      return static_cast<seqan3::aa10li>(in);
                  }));
}

// Two instances of algo_pre exist
// They type erase the different arguments to the same type and encode the type information as a run-time parameter
void algo_pre(seqan3::aa10li_vector const & v)
{
    std::vector<seqan3::semialphabet_any<10>> tmp = v
                                                  | std::views::transform(
                                                        [](auto const & in)
                                                        {
                                                            return static_cast<seqan3::semialphabet_any<10>>(in);
                                                        })
                                                  | seqan3::ranges::to<std::vector>();
    algorithm(tmp, false);
}

void algo_pre(seqan3::aa10murphy_vector const & v)
{
    std::vector<seqan3::semialphabet_any<10>> tmp = v
                                                  | std::views::transform(
                                                        [](auto const & in)
                                                        {
                                                            return static_cast<seqan3::semialphabet_any<10>>(in);
                                                        })
                                                  | seqan3::ranges::to<std::vector>();
    algorithm(tmp, true);
}

int main()
{
    seqan3::aa10li_vector v1{"AVRSTXOUB"_aa10li};
    algo_pre(v1); // BIKBBBKCB

    seqan3::aa10murphy_vector v2{"AVRSTXOUB"_aa10murphy};
    algo_pre(v2); // BIKSSSKCB
}
