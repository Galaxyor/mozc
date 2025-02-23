// Copyright 2010-2021, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MOZC_BASE_UTIL_H_
#define MOZC_BASE_UTIL_H_

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <string>
#include <string_view>
#include <vector>

#include "absl/base/attributes.h"
#include "absl/strings/string_view.h"

namespace mozc {

class Util {
 public:
  Util() = delete;
  ~Util() = delete;

  // String utils
  static void SplitStringToUtf8Chars(absl::string_view str,
                                     std::vector<std::string> *output);

  // Split `str` to graphemes.
  // A grapheme may contain multiple characters such as modifiers and variation
  // squesnces (e.g. 神︀ = U+795E,U+FE00 [SVS]).
  // Note, this function does not support full requirements of the grapheme
  // specifications defined by Unicode.
  static void SplitStringToUtf8Graphemes(absl::string_view str,
                                         std::vector<std::string> *graphemes);

  static void SplitCSV(absl::string_view input,
                       std::vector<std::string> *output);

  static void AppendStringWithDelimiter(absl::string_view delimiter,
                                        absl::string_view append_string,
                                        std::string *output);

  static void LowerString(std::string *str);
  static void UpperString(std::string *str);

  // Transforms the first character to the upper case and tailing characters to
  // the lower cases.  ex. "abCd" => "Abcd".
  static void CapitalizeString(std::string *str);

  // Returns true if the characters in [first, last) are all in lower case
  // ASCII.
  static bool IsLowerAscii(absl::string_view s);

  // Returns true if the characters in [first, last) are all in upper case
  // ASCII.
  static bool IsUpperAscii(absl::string_view s);

  // Returns true if the text in the rage [first, last) is capitalized ASCII.
  static bool IsCapitalizedAscii(absl::string_view s);

  ABSL_DEPRECATED("Use strings::OneCharLen")
  static size_t OneCharLen(const char *src);

  // Returns the lengths of [src, src+size] encoded in UTF8.
  static size_t CharsLen(const char *src, size_t size);

  static size_t CharsLen(absl::string_view str) {
    return CharsLen(str.data(), str.size());
  }

  // Converts a UTF-8 string to UTF-32.
  static std::u32string Utf8ToUtf32(absl::string_view str);
  // Converts a UTF-32 string to UTF-8.
  static std::string Utf32ToUtf8(std::u32string_view str);

  // Converts the first character of UTF8 string starting at |begin| to UCS4.
  // The read byte length is stored to |mblen|.
  static char32_t Utf8ToUcs4(const char *begin, const char *end, size_t *mblen);
  static char32_t Utf8ToUcs4(absl::string_view s) {
    size_t mblen = 0;
    return Utf8ToUcs4(s.data(), s.data() + s.size(), &mblen);
  }

  // Converts a UCS4 code point to UTF8 string.
  static void Ucs4ToUtf8(char32_t c, std::string *output);

  // Converts a UCS4 code point to UTF8 string and appends it to |output|, i.e.,
  // |output| is not cleared.
  static void Ucs4ToUtf8Append(char32_t c, std::string *output);

  // Converts a UCS4 code point to UTF8 and stores it to char array.  The result
  // is terminated by '\0'.  Returns the byte length of converted UTF8 string.
  // REQUIRES: The output buffer must be longer than 7 bytes.
  static size_t Ucs4ToUtf8(char32_t c, char *output);

  // Returns true if |s| is split into |first_char32| + |rest|.
  // You can pass nullptr to |first_char32| and/or |rest| to ignore the matched
  // value.
  // Returns false if an invalid UTF-8 sequence is prefixed. That is, |rest| may
  // contain any invalid sequence even when this method returns true.
  static bool SplitFirstChar32(absl::string_view s, char32_t *first_char32,
                               absl::string_view *rest);

  // Returns true if |s| is split into |rest| + |last_char32|.
  // You can pass nullptr to |rest| and/or |last_char32| to ignore the matched
  // value.
  // Returns false if an invalid UTF-8 sequence is suffixed. That is, |rest| may
  // contain any invalid sequence even when this method returns true.
  static bool SplitLastChar32(absl::string_view s, absl::string_view *rest,
                              char32_t *last_char32);

  // Returns true if |s| is a valid UTF8.
  static bool IsValidUtf8(absl::string_view s);

#ifdef _WIN32
  // Returns how many wide characters are necessary in UTF-16 to represent
  // given UTF-8 string. Note that the result of this method becomes greater
  // than that of Util::CharsLen if |src| contains any character which is
  // encoded by the surrogate-pair in UTF-16.
  ABSL_DEPRECATED("Use win32::WideCharsLen")
  static size_t WideCharsLen(absl::string_view src);
  // Converts the encoding of the specified string from UTF-8 to UTF-16, and
  // vice versa.
  ABSL_DEPRECATED("Use win32::Utf8ToWide")
  static int Utf8ToWide(absl::string_view input, std::wstring *output);
  ABSL_DEPRECATED("Use win32::Utf8ToWide")
  static std::wstring Utf8ToWide(absl::string_view input);
  ABSL_DEPRECATED("Use win32::WideToUtf8")
  static int WideToUtf8(const wchar_t *input, std::string *output);
  ABSL_DEPRECATED("Use win32::WideToUtf8")
  static int WideToUtf8(const std::wstring &input, std::string *output);
  ABSL_DEPRECATED("Use win32::WideToUtf8")
  static std::string WideToUtf8(const std::wstring &input);
#endif  // _WIN32

  // Extracts a substring range, where both start and length are in terms of
  // UTF8 size. Note that the returned string view refers to the same memory
  // block as the input.
  static absl::string_view Utf8SubString(absl::string_view src, size_t start,
                                         size_t length);
  // This version extracts the substring to the end.
  static absl::string_view Utf8SubString(absl::string_view src, size_t start);

  // Extracts a substring of length |length| starting at |start|.
  // Note: |start| is the start position in UTF8, not byte position.
  static void Utf8SubString(absl::string_view src, size_t start, size_t length,
                            std::string *result);

  // Strip a heading UTF-8 BOM (binary order mark) sequence (= \xef\xbb\xbf).
  static void StripUtf8Bom(std::string *line);

  // return true the line starts with UTF16-LE/UTF16-BE BOM.
  static bool IsUtf16Bom(absl::string_view line);

  // Chop the return characters (i.e. '\n' and '\r') at the end of the
  // given line.
  static bool ChopReturns(std::string *line);

  // Returns true if all chars in input are both defined
  // in full width and half-width-katakana area
  static bool IsFullWidthSymbolInHalfWidthKatakana(absl::string_view input);

  // Returns true if all chars are defined in half-width-katakana area.
  static bool IsHalfWidthKatakanaSymbol(absl::string_view input);

  // Returns true if one or more Kana-symbol characters are in the input.
  static bool IsKanaSymbolContained(absl::string_view input);

  // Returns true if |input| looks like a pure English word.
  static bool IsEnglishTransliteration(absl::string_view value);

  // Returns true if key is an open bracket.  If key is an open bracket,
  // corresponding close bracket is assigned.
  static bool IsOpenBracket(absl::string_view key,
                            absl::string_view *close_bracket);

  // Returns true if key is a close bracket.  If key is a close bracket,
  // corresponding open bracket is assigned.
  // Note, `open_bracket` is not terminated with '\0'.
  static bool IsCloseBracket(absl::string_view key,
                             absl::string_view *open_bracket);

  // Returns true if input is a bracket pair text (e.g. "「」").
  static bool IsBracketPairText(absl::string_view input);

  // Escape any characters into \x prefixed hex digits.
  // ex.  "ABC" => "\x41\x42\x43".
  static void Escape(absl::string_view input, std::string *output);
  static std::string Escape(absl::string_view input);
  static bool Unescape(absl::string_view input, std::string *output);

  enum ScriptType {
    UNKNOWN_SCRIPT,
    KATAKANA,
    HIRAGANA,
    KANJI,
    NUMBER,
    ALPHABET,
    EMOJI,
    SCRIPT_TYPE_SIZE,
  };

  // return script type of w
  static ScriptType GetScriptType(char32_t w);

  // return script type of first character in [begin, end)
  // This function finds the first UTF-8 chars and returns its script type.
  // The length of the character will be returned in *mblen.
  // This function calls GetScriptType(char32_t) internally.
  static ScriptType GetScriptType(const char *begin, const char *end,
                                  size_t *mblen);

  // return script type of first character in str
  static ScriptType GetFirstScriptType(absl::string_view str);

  // return script type of string. all chars in str must be
  // KATAKANA/HIRAGANA/KANJI/NUMBER or ALPHABET.
  // If str has mixed scripts, this function returns UNKNOWN_SCRIPT
  static ScriptType GetScriptType(absl::string_view str);

  // The same as GetScryptType(), but it ignores symbols
  // in the |str|.
  static ScriptType GetScriptTypeWithoutSymbols(absl::string_view str);

  // return true if all script_type in str is "type"
  static bool IsScriptType(absl::string_view str, ScriptType type);

  // return true if the string contains script_type char
  static bool ContainsScriptType(absl::string_view str, ScriptType type);

  // See 'Unicode Standard Annex #11: EAST ASIAN WIDTH'
  // http://www.unicode.org/reports/tr11/
  // http://www.unicode.org/Public/UNIDATA/EastAsianWidth.txt
  enum FormType {
    UNKNOWN_FORM,
    HALF_WIDTH,  // [Na] and [H] in 'Unicode Standard Annex #11'
    FULL_WIDTH,  // Any other characters
    FORM_TYPE_SIZE,
  };

  // return Form type of single character.
  // This function never returns UNKNOWN_FORM.
  static FormType GetFormType(char32_t w);

  // return FormType of string.
  // return UNKNOWN_FORM if |str| contains both HALF_WIDTH and FULL_WIDTH.
  static FormType GetFormType(absl::string_view str);

  // Returns true if all characters of `str` are ASCII (U+00 - U+7F).
  static bool IsAscii(absl::string_view str);

  // Returns true if all characters of `str` are JIS X 0208.
  static bool IsJisX0208(absl::string_view str);

  // Serializes uint64_t into a string of eight byte.
  static std::string SerializeUint64(uint64_t x);

  // Deserializes a string serialized by SerializeUint64.  Returns false if the
  // length of s is not eight or s is in an invalid format.
  static bool DeserializeUint64(absl::string_view s, uint64_t *x);

  // Checks whether the letter is ucs 4 is appropriate target to be shown as
  // candidate. This function is based on mozc internal logics, rather than
  // orthodox classification logics.
  static bool IsAcceptableCharacterAsCandidate(char32_t letter);
};

// Const iterator implementation to traverse on a (utf8) string as a char32
// string.
//
// Example usage:
//   string utf8;
//   for (ConstChar32Iterator iter(utf8); !iter.Done(); iter.Next()) {
//     char32_t c = iter.Get();
//     ...
//   }
class ConstChar32Iterator {
 public:
  explicit ConstChar32Iterator(absl::string_view utf8_string);
  ConstChar32Iterator(const ConstChar32Iterator &) = delete;
  ConstChar32Iterator &operator=(const ConstChar32Iterator &) = delete;
  char32_t Get() const;
  void Next();
  bool Done() const;

 private:
  absl::string_view utf8_string_;
  char32_t current_;
  bool done_;
};

// Const reverse iterator implementation to traverse on a (utf8) string as a
// char32_t string.
//
// Example usage:
//   string utf8;
//   for (ConstChar32ReverseIterator iter(utf8); !iter.Done(); iter.Next()) {
//     char32_t c = iter.Get();
//     ...
//   }
class ConstChar32ReverseIterator {
 public:
  explicit ConstChar32ReverseIterator(absl::string_view utf8_string);
  ConstChar32ReverseIterator(const ConstChar32ReverseIterator &) = delete;
  ConstChar32ReverseIterator &operator=(const ConstChar32ReverseIterator &) =
      delete;
  char32_t Get() const;
  void Next();
  bool Done() const;

 private:
  absl::string_view utf8_string_;
  char32_t current_;
  bool done_;
};

}  // namespace mozc

#endif  // MOZC_BASE_UTIL_H_
