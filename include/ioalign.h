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

    /** @brief Manipulator that calls basic_align_proxy::hline.
     *
     * For example:
     *
     *     proxy << io::hline;
     */
    template<typename A>
    basic_align_proxy<A>& hline(basic_align_proxy<A>& os);

    /** @brief Manipulator that calls basic_align_proxy::endr.
     *
     * For example:
     *
     *     proxy << io::endr;
     */
    template<typename A>
    basic_align_proxy<A>& endr(basic_align_proxy<A>& os);

    /** @brief Manipulator that calls basic_align_proxy::heads.
     *
     * For example:
     *
     *     proxy << io::heads;
     *
     */
    template<typename A>
    basic_align_proxy<A>& heads(basic_align_proxy<A>& os);

    /** @brief Manipulator that calls basic_align_proxy::tab.
     *
     * For example:
     *
     *     proxy << "hello" << io::tab << "world" << io::endr;
     */
    template<typename A>
    basic_align_proxy<A>& tab(basic_align_proxy<A>& os);

    /** @brief Manipulator that calls basic_align_proxy::resetheads.
     *
     * For example:
     *
     *     proxy << io::resetheads;
     */
    template<typename A>
    basic_align_proxy<A>& resetheads(basic_align_proxy<A>& os);

    /** @brief Manipulator that calls basic_align_proxy::reset.
     *
     * For example:
     *
     *     proxy << io::reset;
     */
    template<typename A>
    basic_align_proxy<A>& reset(basic_align_proxy<A>& os);

    /// @cond hidden
    template<typename Char>
    class sethead_;
    /// @endcond

    /** @brief Manipulator object to help call basic_align_proxy::sethead.
     *
     * For example:
     *
     *     proxy << io::head("first", 10) << io::head("second") << io::endr;
     *
     * The second argument is optional; if set it indicates the
     * minimum column width.
     */
    template<typename Char>
    sethead_<Char> head(const Char* s, unsigned width = 0);

    /** @brief Manipulator object to help call basic_align_proxy::sethead.
     *
     * For example:
     *
     *     std::string h = "first";
     *     proxy << io::head(h, 10) << io::endr;
     *
     */
    template<typename Char, typename Traits>
    sethead_<Char> head(const std::basic_string<Char, Traits>& s, unsigned width = 0);

    /// @cond hidden
    template<typename Char, bool heads>
    class raw_;
    /// @endcond

    /** @brief Manipulator object to help call basic_align_proxy::raw.
     *
     * For example:
     *
     *     proxy << io::raw("hello\tworld\nsecond\trow");
     */
    template<typename Char>
    raw_<Char, false>
    raw(const Char *s);

    /** @brief Manipulator object to help call basic_align_proxy::raw.
     *
     * For example:
     *
     *     std::string t = "hello\tworld\nsecond\trow";
     *     proxy << io::raw(t);
     */
    template<typename Char, typename Traits>
    raw_<Char, false>
    raw(const std::basic_string<Char, Traits>& s);

    /** @brief Manipulator object to help call basic_align_proxy::rawheads.
     *
     * For example:
     *
     *     proxy << io::rawheads("hello\tworld\nsecond\trow");
     */
    template<typename Char>
    raw_<Char, true>
    rawheads(const Char *s);

    /** @brief Manipulator object to help call basic_align_proxy::rawheads.
     *
     * For example:
     *
     *     std::string t = "hello\tworld\nsecond\trow";
     *     proxy << io::rawheads(t);
     */
    template<typename Char, typename Traits>
    raw_<Char, true>
    rawheads(const std::basic_string<Char, Traits>& s);

    template<typename Align>
    class basic_align_proxy {
    public:
        typedef typename Align::stream_type     stream_type;
        typedef typename stream_type::char_type char_type;
        typedef typename stream_type::pos_type  pos_type;
        typedef typename Align::string_type     string_type;
        typedef typename string_type::size_type size_type;

        /** @brief Parse a C string and interpret tabs and newlines as table control.
         * @param cstr The nul-terminated C string to parse.
         *
         * For example:
         *
         *     p.raw("hello\tworld\n");
         *
         * Is equivalent to:
         *
         *     p << "hello" << io::tab << "world" << io::endr;
         */
        void
        raw(const char_type* cstr);

        /** @brief Parse a C string for column headers.
         * @param cstr The nul-terminated C string to parse.
         *
         * For example:
         *
         *     p.rawheads("hello\tworld\n");
         *
         * Is equivalent to:
         *
         *     p << io::head("hello") << io::head("world") << io::endr;
         */
        void
        rawheads(const char_type* cstr);

        /** @brief Sets a column header title.
         * @param cstr The C string to use.
         * @param min_width The minimum column width, if desired.
         * @param slen The limit on the size of the C string, if known.
         *
         * If `min_width` is zero, the minimum width is the size of the
         * column title.  If `slen` is string_type::npos, the entire C string is
         * used.
         *
         * @see basic_align_proxy::heads
         */
        void
        sethead(const char_type* cstr, unsigned min_width = 0,
                size_type slen = string_type::npos);

        /// Complete the row with column headers.
        void heads();

        /// Complete the row with a horizontal rule.
        void hline();

        /// Tabulate to the next column.
        void tab();

        /** @brief Complete the current row and start a new row.
         * If the current row is empty, endr() has no effect.
         */
        void endr();

        /// Reset the column widths and erase the column headers.
        void reset();

        /// Erase the column headers.
        void resetheads();

        /// Output a value to the attached stream.
        template<typename T>
        basic_align_proxy&
        operator<<(const T& t);

        /// Call basic_align_proxy::sethead.
        basic_align_proxy&
        operator<<(const sethead_<char_type>& h);

        /// Call basic_align_proxy::raw.
        basic_align_proxy&
        operator<<(const raw_<char_type, false>& h);

        /// Call basic_align_proxy::rawheads.
        basic_align_proxy&
        operator<<(const raw_<char_type, true>& h);

        /// Destroy the proxy and release the attached stream.
        ~basic_align_proxy();

        /// Set the column fill character.
        void setfill(char_type fill = ' ');
        /// Set the column separator character.
        void setsep(char_type sep = ' ');
        /// Set the horizontal rule character.
        void setrule(char_type rule = '-');
    private:
        stream_type& os_;
        Align&       a_;
        unsigned     col_;
        pos_type     cursor_;
        pos_type     last_pos_;
        bool         at_begin_;
        std::locale  prev_locale_;
        char_type    fill_char_;
        char_type    sep_char_;
        char_type    rule_char_;

        template<typename O>
        friend class basic_align;

        basic_align_proxy(stream_type &os, Align& a);

        template<typename A>
        friend
        basic_align_proxy<A>&
        operator<<(basic_align_proxy<A>& s,
                   typename A::stream_type& (*op)(typename A::stream_type&));

        unsigned
        pre_tab();

        void
        complete_column();

        void
        complete_row();

        bool
        at_column_start();
    };


    template<typename OStream>
    class basic_align
    {
    public:
        typedef OStream                                   stream_type;
        typedef basic_align_proxy<basic_align<OStream> >  proxy_type;
        typedef typename OStream::char_type               char_type;
        typedef typename OStream::traits_type             traits_type;
        typedef std::basic_string<char_type, traits_type> string_type;

        proxy_type
        attach(stream_type& os);

        basic_align();

    private:
        std::vector<int>         widths_;
        std::vector<string_type> heads_;

        template<typename A>
        friend class basic_align_proxy;
    };

    typedef basic_align<std::ostream> align;
    typedef basic_align_proxy<align> align_proxy;

    /// @cond IMPLEMENTATION

    template<typename A>
    bool basic_align_proxy<A>::at_column_start()
    {
        os_.flush();
        return cursor_ == last_pos_;
    }

    template<typename A>
    void basic_align_proxy<A>::setfill(typename basic_align_proxy<A>::char_type fill_char)
    {
        fill_char_ = fill_char;
    }

    template<typename A>
    void basic_align_proxy<A>::setsep(typename basic_align_proxy<A>::char_type sep_char)
    {
        sep_char_ = sep_char;
    }

    template<typename A>
    void basic_align_proxy<A>::setrule(typename basic_align_proxy<A>::char_type rule_char)
    {
        rule_char_ = rule_char;
    }

    template<typename A>
    void basic_align_proxy<A>::resetheads()
    {
        a_.heads_.clear();
    }

    template<typename A>
    void basic_align_proxy<A>::reset()
    {
        resetheads();
        a_.widths_.clear();
    }

    template<typename A>
    void basic_align_proxy<A>::endr()
    {
        bool acs = at_column_start();
        bool skip_newline = at_begin_ && acs;

        if (!skip_newline)
        {
            pre_tab();
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
    unsigned
    basic_align_proxy<A>::pre_tab()
    {
        // Ensure there is enough room in the
        // widths array for the current column.
        if (col_ >= a_.widths_.size())
            a_.widths_.resize(col_ + 1);

        // See where we are in the stream.
        pos_type cur_pos = cursor_;
        int w = cur_pos - last_pos_;
        if (w < 0)
            w = 0;

        // Adjust the current known width.
        int prev_w = a_.widths_[col_];
        if (w > prev_w)
            a_.widths_[col_] = prev_w = w;

        return prev_w - w;
    }


    template<typename A>
    void basic_align_proxy<A>::complete_column()
    {
        unsigned remainder = pre_tab();

        // Pad until end of column.
        for (unsigned i = 0; i < remainder; ++i)
            os_ << fill_char_;

        // Separator.
        os_ << sep_char_;

        // Adjust column.
        ++col_;
    }

    template<typename A>
    void basic_align_proxy<A>::complete_row()
    {
        // Complete the row.
        os_ << '\n';

        // Update cursor.
        os_.flush();

        // Start new row.
        col_ = 0;
        last_pos_ = cursor_;
        at_begin_ = true;
    }

    template<typename A>
    void basic_align_proxy<A>::tab()
    {
        // Complete I/O, needed by complete_column().
        os_.flush();

        // Pad until end of column.
        complete_column();

        // Update cursor.
        os_.flush();

        // Move the cursors forward.
        at_begin_ = false;
        last_pos_ = cursor_;
    }

    template<typename A>
    void basic_align_proxy<A>::hline()
    {
        if (col_ + 1 < a_.widths_.size())
        {
            if (!at_column_start())
                complete_column();

            // Fill the remainder of the row with a rule.
            for (unsigned i = col_; i < a_.widths_.size(); ++i)
            {
                for (unsigned j = 0; j < a_.widths_[i]; ++j)
                    os_ << rule_char_;
                if (i + 1 < a_.widths_.size())
                    os_ << sep_char_;
            }

            complete_row();
        }
        else
            endr();
    }

    template<typename A>
    void basic_align_proxy<A>::heads()
    {
        if (col_ + 1 < a_.widths_.size())
        {
            if (!at_column_start())
                complete_column();

            // Print out the headers.
            for (unsigned i = col_; i < a_.heads_.size(); ++i)
            {
                os_ << a_.heads_[i];
                if (i + 1 < a_.heads_.size())
                {
                    for (unsigned j = 0; j < a_.widths_[i] - a_.heads_[i].size(); ++j)
                        os_ << fill_char_;
                    os_ << sep_char_;
                }
            }
            complete_row();
        }
        else
            endr();
    }

    template<typename A>
    void
    basic_align_proxy<A>::sethead(const typename basic_align_proxy<A>::char_type* s,
                                  unsigned w, size_type len)
    {
        // Place the label in the heads array.
        if (col_ >= a_.heads_.size())
            a_.heads_.resize(col_ + 1);

        if (len != string_type::npos)
            a_.heads_[col_].assign(s, len);
        else
            a_.heads_[col_].assign(s);

        size_type clen = w > a_.heads_[col_].size() ? w : a_.heads_[col_].size();
        // Now adjust the widths.
        if (col_ >= a_.widths_.size())
            a_.widths_.resize(col_ + 1);
        if (clen > a_.widths_[col_])
            a_.widths_[col_] = clen;

        // fprintf(debug, "HAI head:%s:%d:\n", a_.heads_[col_].c_str(), (int)a_.widths_[col_]);

        // Adjust column.
        ++col_;
    }

    template<typename A>
    void
    basic_align_proxy<A>::rawheads(const typename basic_align_proxy<A>::char_type* s)
    {
        size_type i, laststart;

        for (laststart = 0, i = 0; s[i] != '\0'; ++i)
        {
            if (s[i] != '\t' && s[i] != '\n')
                continue;

            sethead(s + laststart, i - laststart, i - laststart);

            if (s[i] == '\n')
                endr();

            laststart = i + 1;
        }
        if (i > laststart)
            sethead(s + laststart, i - laststart, i - laststart);
    }

    template<typename A>
    void
    basic_align_proxy<A>::raw(const typename basic_align_proxy<A>::char_type* s)
    {
        size_type i, laststart;

        for (laststart = 0, i = 0; s[i] != '\0'; ++i)
        {
            if (s[i] != '\t' && s[i] != '\n')
                continue;

            os_.write(s + laststart, i - laststart);

            if (s[i] == '\n')
                endr();
            else
                tab();

            laststart = i + 1;
        }
        if (i > laststart)
            os_.write(s + laststart, i - laststart);
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
    reset(basic_align_proxy<A>& os)
    {
        os.reset();
        return os;
    }

    template<typename A>
    inline basic_align_proxy<A>&
    resetheads(basic_align_proxy<A>& os)
    {
        os.resetheads();
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
        unsigned w_;

        template<typename A>
        friend class basic_align_proxy;

    public:
        sethead_(const Char* s, unsigned w) : s_(s), w_(w) {}
    };

    template<typename Char>
    inline sethead_<Char>
    head(const Char* s, unsigned width)
    {
        return sethead_<Char>(s, width);
    }
    template<typename Char, typename Traits>
    inline sethead_<Char>
    head(const std::basic_string<Char, Traits>& s, unsigned width)
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


    template<typename Char, bool heads>
    class raw_ {
        const Char* s_;

        template<typename A>
        friend class basic_align_proxy;

    public:
        raw_(const Char* s) : s_(s) {}
    };

    template<typename Char>
    inline raw_<Char, false>
    raw(const Char *s)
    {
        return raw_<Char, false>(s);
    }
    template<typename Char, typename Traits>
    inline raw_<Char, false>
    raw(const std::basic_string<Char, Traits>& s)
    {
        return raw_<Char, false>(s.c_str());
    }
    template<typename Char>
    inline raw_<Char, true>
    rawheads(const Char *s)
    {
        return raw_<Char, true>(s);
    }
    template<typename Char, typename Traits>
    inline raw_<Char, true>
    rawheads(const std::basic_string<Char, Traits>& s)
    {
        return raw_<Char, true>(s.c_str());
    }

    template<typename A>
    inline basic_align_proxy<A>&
    basic_align_proxy<A>::operator<<(const raw_<typename basic_align_proxy<A>::char_type, false>& r)
    {
        raw(r.s_);
        return *this;
    }
    template<typename A>
    inline basic_align_proxy<A>&
    basic_align_proxy<A>::operator<<(const raw_<typename basic_align_proxy<A>::char_type, true>& r)
    {
        rawheads(r.s_);
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
                                            typename stream_type::traits_type::state_type>(cursor_)))),
          fill_char_(' '),
          sep_char_(' '),
          rule_char_('-')
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
