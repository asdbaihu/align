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

#include <locale>
#include <vector>
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

    /** @brief Manipulator that completes the row with a horizontal rule.
     *
     * For example:
     *
     *     proxy << io::hline;
     *
     * This manipulator also terminates the row and prepares a new
     * row.
     */
    template<typename A>
    basic_align_proxy<A>& hline(basic_align_proxy<A>& os);

    /** @brief Manipulator that terminates the current row.
     *
     * For example:
     *
     *     proxy << io::endr;
     */
    template<typename A>
    basic_align_proxy<A>& endr(basic_align_proxy<A>& os);

    /** @brief Manipulator that completes a row with headers.
     *
     * For example:
     *
     *     proxy << io::heads;
     *
     * This manipulator also terminates the row and prepares a new
     * row.
     */
    template<typename A>
    basic_align_proxy<A>& heads(basic_align_proxy<A>& os);

    /** @brief Manipulator that moves output to the next column,
     * adding padding as necessary.
     *
     * For example:
     *
     *     proxy << "hello" << io::tab << "world" << io::endr;
     */
    template<typename A>
    basic_align_proxy<A>& tab(basic_align_proxy<A>& os);

    /// @cond hidden
    template<typename Char>
    class sethead_;
    /// @endcond

    /** @brief Set a column header.
     *
     * For example:
     *
     *     proxy << io::head("first", 10) << io::head("second") << io::endr;
     *
     * The second argument is optional; if set it indicates the
     * minimum column width.
     */
    template<typename Char>
    sethead_<Char> head(const Char* s, size_t width = 0);

    template<typename Char, typename Traits>
    sethead_<Char> head(const std::basic_string<Char, Traits>& s,
                   size_t width = 0);

    /// @cond hidden
    template<typename Char>
    class raw_;
    /// @endcond

    /** @brief Feed a C string and interprets tab and newline characters.
     *
     * For example:
     *
     *     proxy << io::raw("hello\tworld\n");
     */
    template<typename Char>
    raw_<Char> raw(const Char *s,
                   bool first_row_defines_heads = false);

    template<typename Char, typename Traits>
    raw_<Char> raw(const std::basic_string<Char, Traits>& s,
                   bool first_row_defines_heads = false);

    template<typename Align>
    class basic_align_proxy {
    public:
        typedef typename Align::stream_type     stream_type;
        typedef typename stream_type::char_type char_type;
        typedef typename stream_type::pos_type  pos_type;

        void
        raw(const char_type* r, bool first_row_heads);

        void
        sethead(const char_type* h, size_t w, size_t len = 0);

        void
        heads();

        void
        hline();

        void
        tab(bool pad = true, char_type pad_char = ' ');

        void
        endr(bool reset_only = false);

        template<typename T>
        basic_align_proxy&
        operator<<(const T& t);

        basic_align_proxy&
        operator<<(const sethead_<char_type>& h);

        basic_align_proxy&
        operator<<(const raw_<char_type>& h);

        ~basic_align_proxy();

    private:
        stream_type& os_;
        Align&       a_;
        unsigned     col_;
        pos_type     cursor_;
        pos_type     last_pos_;
        bool         at_begin_;
        std::locale  prev_locale_;

        template<typename O>
        friend class basic_align;

        basic_align_proxy(stream_type &os, Align& a);

        template<typename A>
        friend
        basic_align_proxy<A>&
        operator<<(basic_align_proxy<A>& s,
                   typename A::stream_type& (*op)(typename A::stream_type&));
    };


    template<typename OStream>
    class basic_align
    {
    public:
        typedef OStream                                              stream_type;
        typedef basic_align_proxy<basic_align<OStream> >             proxy_type;

        proxy_type
        attach(stream_type& os);

        basic_align();

    private:
        std::vector<int> widths_;
        std::vector<std::basic_string<typename OStream::char_type,
                                      typename OStream::traits_type> > heads_;

        template<typename A>
        friend class basic_align_proxy;
    };

    typedef basic_align<std::ostream> align;
    typedef basic_align_proxy<align> align_proxy;

    /// @cond IMPLEMENTATION

    template<typename A>
    void basic_align_proxy<A>::endr(bool reset_only)
    {
        os_.flush();
        bool skip_newline = at_begin_ && (cursor_ == last_pos_);

        if (!reset_only && !skip_newline)
        {
            tab(false);
            // for (int i = 0; i < a_.widths_.size(); ++i) fprintf(debug, ".%d", (int)a_.widths_[i]);
            os_ << '\n';
            os_.flush();
        }

        // Start new row
        col_ = 0;
        last_pos_ = cursor_;
        at_begin_ = true;
    }

    template<typename A>
    void basic_align_proxy<A>::tab(bool pad,
                                   typename basic_align_proxy<A>::char_type pad_char)
    {
        // Ensure there is enough room in the
        // widths array for the current column.
        if (col_ >= a_.widths_.size())
            a_.widths_.resize(col_ + 1);

        // See where we are in the stream.
        os_.flush();
        pos_type cur_pos = cursor_;
        int w = cur_pos - last_pos_;
        if (w < 0)
            w = 0;

        // Adjust the current known width.
        int prev_w = a_.widths_[col_];
        if (w > prev_w)
            a_.widths_[col_] = w;

        // Pad if necessary.
        if (pad)
        {
            for (int i = 0; i < prev_w - w; ++i)
                os_ << pad_char;
            os_ << ' ';
            os_.flush();
        }

        // Move the cursors forward.
        ++col_;
        at_begin_ = false;
        last_pos_ = cursor_;
    }

    template<typename A>
    void basic_align_proxy<A>::hline()
    {
        for (unsigned i = col_; i < a_.widths_.size(); ++i)
            tab(true, '-');
        os_ << '\n';

        endr(true);
    }

    template<typename A>
    void basic_align_proxy<A>::heads()
    {
        // fprintf(debug, "HAI head:%s:%d:\n", a_.heads_[col_].c_str(), (int)a_.widths_[col_]);
        for (unsigned i = col_; i < a_.heads_.size(); ++i)
        {
            os_ << a_.heads_[i];
            if (i < a_.heads_.size() - 1)
                tab();
        }
        endr();
    }

    template<typename A>
    void
    basic_align_proxy<A>::sethead(const typename basic_align_proxy<A>::char_type* s,
                                  size_t w, size_t len)
    {
        // Place the label in the heads array.
        if (col_ >= a_.heads_.size())
            a_.heads_.resize(col_ + 1);

        if (len != 0)
            a_.heads_[col_].assign(s, len);
        else
            a_.heads_[col_].assign(s);

        // Now adjust the widths.
        if (col_ >= a_.widths_.size())
            a_.widths_.resize(col_ + 1);
        a_.widths_[col_] = w > a_.heads_[col_].size() ? w : a_.heads_[col_].size();

        // fprintf(debug, "HAI head:%s:%d:\n", a_.heads_[col_].c_str(), (int)a_.widths_[col_]);

        // Adjust column.
        ++col_;
    }

    template<typename A>
    void
    basic_align_proxy<A>::raw(const typename basic_align_proxy<A>::char_type* s,
                              bool fh)
    {
        size_t i, laststart;
        bool firstrow = true;

        for (laststart = 0, i = 0; s[i] != '\0'; ++i)
        {
            if (s[i] != '\t' && s[i] != '\n')
                continue;

            if (i > laststart)
            {
                if (fh && firstrow)
                    sethead(s + laststart, i - laststart, i - laststart);
                else
                    os_.write(s + laststart, i - laststart);
            }

            if (fh && firstrow)
            {
                if (s[i] == '\n')
                {
                    endr();
                    firstrow = false;
                }
            }
            else
            {
                if (s[i] == '\t')
                    tab();
                else
                    endr();
            }

            laststart = i + 1;
        }
        if (i > laststart)
        {
            if (fh && firstrow)
                sethead(s + laststart, i - laststart, i - laststart);
            else
                os_.write(s + laststart, i - laststart);
        }
    }

    template<typename A>
    inline basic_align_proxy<A>&
    hline(basic_align_proxy<A>& os)
    {
        os.hline();
        return os;
    }

    template<typename A>
    inline basic_align_proxy<A>&
    endr(basic_align_proxy<A>& os)
    {
        os.endr();
        return os;
    }

    template<typename A>
    inline basic_align_proxy<A>&
    tab(basic_align_proxy<A>& os)
    {
        os.tab();
        return os;
    }

    template<typename A>
    inline basic_align_proxy<A>&
    heads(basic_align_proxy<A>& os)
    {
        os.heads();
        return os;
    }

    template<typename Char>
    class sethead_ {
        const Char* s_;
        size_t w_;

        template<typename A>
        friend class basic_align_proxy;

    public:
        sethead_(const Char* s, size_t w) : s_(s), w_(w) {}
    };

    template<typename Char>
    inline sethead_<Char>
    head(const Char* s, size_t width)
    {
        return sethead_<Char>(s, width);
    }
    template<typename Char, typename Traits>
    inline sethead_<Char>
    head(const std::basic_string<Char, Traits>& s, size_t width)
    {
        return sethead_<Char>(s.c_str(), width);
    }

    template<typename A>
    inline basic_align_proxy<A>&
    basic_align_proxy<A>::operator<<(const sethead_<typename basic_align_proxy<A>::char_type>& h)
    {
        sethead(h.s_, h.w_);
        return *this;
    }


    template<typename Char>
    class raw_ {
        const Char* s_;
        bool has_heads_;

        template<typename A>
        friend class basic_align_proxy;

    public:
        raw_(const Char* s, bool has_heads) : s_(s), has_heads_(has_heads) {}
    };

    template<typename Char>
    inline raw_<Char>
    raw(const Char *s, bool has_heads)
    {
        return raw_<Char>(s, has_heads);
    }
    template<typename Char, typename Traits>
    inline raw_<Char>
    raw(const std::basic_string<Char, Traits>& s, bool has_heads)
    {
        return raw_<Char>(s.c_str(), has_heads);
    }

    template<typename A>
    inline basic_align_proxy<A>&
    basic_align_proxy<A>::operator<<(const raw_<typename basic_align_proxy<A>::char_type>& r)
    {
        raw(r.s_, r.has_heads_);
        return *this;
    }


    template<typename Pos, typename Char, typename State>
    class ocounter: public std::codecvt<Char, Char, State>
    {
    public:
        ocounter(Pos& count) : count_(&count) {}
    private:
        typedef std::codecvt<Char,Char,State> parent;
        typedef typename parent::result       result;

        virtual bool do_always_noconv() const throw()
        {
            return false;
        }
        virtual result do_out (State& state,
                               const Char* fr, const Char* fe, const Char*& fn,
                               Char*       to, Char*       te, Char*&       tn) const
        {
            (*count_) += (fe - fr);
            return parent::do_out(state, fr, fe, fn, to, te, tn);
        }

        Pos* count_;
    };



    template<typename Align>
    basic_align_proxy<Align>::basic_align_proxy(typename Align::stream_type& os, Align& a)
        : os_(os), a_(a), col_(0), cursor_(0),
          last_pos_(0),
          at_begin_(true),
          prev_locale_(os.imbue(std::locale(os.getloc(),
                                            new ocounter<pos_type, char_type,
                                            typename stream_type::traits_type::state_type>(cursor_))))
    {
    }

    template<typename Align>
    basic_align_proxy<Align>::~basic_align_proxy()
    {
        os_.imbue(prev_locale_);
    }

    template<typename A>
    inline basic_align_proxy<A>&
    operator<<(basic_align_proxy<A>& s,
               typename A::stream_type& (*op)(typename A::stream_type&))
    {
        op(s.os_);
        return s;
    };

    template<typename A>
    inline basic_align_proxy<A>&
    operator<<(basic_align_proxy<A>& s,
               basic_align_proxy<A>& (*op)(basic_align_proxy<A>&))
    {
        return op(s);
    };


    template<typename A>
    template<typename T>
    basic_align_proxy<A>&
    basic_align_proxy<A>::operator<<(const T& t)
    {
        os_ << t;
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
        : widths_(), heads_()
    {
    }

    /// @endcond

}

#endif
