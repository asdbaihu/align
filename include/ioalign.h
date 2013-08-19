// io::align -- column alignment for streams -*- C++ -*-
//
// Copyright (c) 2013 Raphael 'kena' Poss
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef _IO_ALIGN_H
#define _IO_ALIGN_H

#include <string>
#include <ios>

/// Namespace for io::align.
namespace io
{
    /// Main class: holds the alignment state across stream output operations.
    template<typename OStream>
    class basic_align;

    /// Output proxy: interfaces between basic_align and the output stream.
    template<typename Align>
    class basic_align_proxy;

    /** @brief Manipulator that emits a horizontal rule using the full table width.
     *
     * For example:
     *
     *     proxy << io::hline;
     *
     * An horizontal rule first ends the current row, then also prepares a new row.
     */
    template<typename OStream>
    OStream& hline(OStream& os);

    /** @brief Manipulator that ends the current row.
     *
     * For example:
     *
     *     proxy << io::endr;
     */
    template<typename OStream>
    OStream& endr(OStream& os);

    /** @brief Manipulator that moves output to the next column, adding padding as necessary.
     *
     * For example:
     *
     *     proxy << "hello" << io::tab << "world" << io::endr;
     */
    template<typename OStream>
    OStream& tab(OStream& os);

    /** @brief Manipulator that marks the next column outputs as headers.
     *
     * For example:
     *
     *     proxy << io::heads << "first" << io::tab << "second" << io::endr;
     */
    template<typename OStream>
    OStream& heads(OStream& os);


    /// @cond hidden
    template<typename Char>
    struct raw_;
    /// @endcond

    /** @brief Manipulator that feeds a C string and interprets tab and newline characters.
     *
     * For example:
     *
     *     proxy << io::raw("hello\tworld\n");
     */
    template<typename Char>
    raw_<Char> raw(const Char *s);

    /** @brief Manipulator that feeds a standard string and interprets tab and newline characters.
     *
     * For example:
     *
     *     std::string s = "hello\tworld\n";
     *     proxy << io::raw(s);
     */
    template<typename Char, typename Traits>
    raw_<Char> raw(const std::basic_string<Char, Traits>& s);

    template<typename Align>
    class basic_align_proxy {
    public:
        typedef typename Align::stream_type     stream_type;
        typedef typename Align::char_type       char_type;

        /// Forwards an output operation to the proxied stream directly.
        template<typename T>
        basic_align_proxy& operator<<(const T& t);

        /// Applies a manipulator and detects hline, tab and endr.
        basic_align_proxy& operator<<(stream_type& (*op)(stream_type&));

        /// Feeds a character string to basic_align::output_raw.
        basic_align_proxy& operator<<(const raw_<char_type>& r);

    private:
        stream_type& os_;
        Align&       a_;

        basic_align_proxy(stream_type &os, Align& a);
        template<typename T> friend class basic_align;
    };


    template<typename OStream>
    class basic_align
    {
    public:
        typedef OStream                                              stream_type;
        typedef typename OStream::char_type                          char_type;
        typedef std::basic_string<typename stream_type::char_type,
                                  typename stream_type::traits_type> string_type;
        typedef basic_align_proxy<basic_align<OStream> >             proxy_type;

        proxy_type
        attach(stream_type& os);

        void
        output_raw(stream_type& os, const char_type* r);

        void
        output_hline(stream_type& os);

        void
        output_tab(stream_type& os);

        void
        output_endr(stream_type& os);

        void
        reset_heads();

        basic_align();
    };

    typedef basic_align<std::ostream> align;
    typedef basic_align_proxy<align> align_proxy;


    /// @cond IMPLEMENTATION


    template<typename OStream>
    OStream& hline(OStream& os)
    {
        return os << "--\n";
    }

    template<typename OStream>
    OStream& tab(OStream& os)
    {
        return os << '\t';
    }

    template<typename OStream>
    OStream& endr(OStream& os)
    {
        return os << '\n';
    }

    template<typename OStream>
    OStream& heads(OStream& os)
    {
        return os;
    }

    template<typename Char>
    struct raw_ {
        raw_(const Char* s) : s_(s) {}
        const Char* s_;
    };

    template<typename Char>
    raw_<Char> raw(const Char *s)
    {
        return raw_<Char>(s);
    }
    template<typename Char, typename Traits>
    raw_<Char> raw(const std::basic_string<Char, Traits>& s)
    {
        return raw_<Char>(s.c_str());
    }

    template<typename Align>
    basic_align_proxy<Align>::basic_align_proxy(typename Align::stream_type& os, Align& a)
        : os_(os), a_(a)
    {}

    template<typename Align>
    template<typename T>
    basic_align_proxy<Align>&
    basic_align_proxy<Align>::operator<<(const T& t)
    {
        os_ << t;
        return *this;
    }

    template<typename Align>
    basic_align_proxy<Align>&
    basic_align_proxy<Align>::operator<<(const raw_<typename Align::char_type>& r)
    {
        a_.output_raw(os_, r.s_);
        return *this;
    }

    template<typename Align>
    basic_align_proxy<Align>&
    basic_align_proxy<Align>::operator<<(typename Align::stream_type& (*manip)(typename Align::stream_type&))
    {
        if (manip == &hline<stream_type>)
            a_.output_hline(os_);
        else if (manip == &tab<stream_type>)
            a_.output_tab(os_);
        else if (manip == &endr<stream_type>)
            a_.output_endr(os_);
        else if (manip == &heads<stream_type>)
            a_.reset_heads();
        else
            os_ << manip;
        return *this;
    }

    template<typename OStream>
    typename basic_align<OStream>::proxy_type
    basic_align<OStream>::attach(OStream& os)
    {
        return proxy_type(os, *this);
    }

    template<typename OStream>
    basic_align<OStream>::basic_align()
    {
        // FIXME
    }

    /// @endcond

}

#endif
