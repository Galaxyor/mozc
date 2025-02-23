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

#include "config/character_form_manager.h"

#include <optional>
#include <string>

#include "base/system_util.h"
#include "config/config_handler.h"
#include "protocol/config.pb.h"
#include "testing/googletest.h"
#include "testing/gunit.h"
#include "absl/flags/flag.h"

namespace mozc {
namespace config {

class CharacterFormManagerTest : public ::testing::Test {
 public:
  void SetUp() override {
    // set default user profile directory
    SystemUtil::SetUserProfileDirectory(absl::GetFlag(FLAGS_test_tmpdir));
    CharacterFormManager *manager =
        CharacterFormManager::GetCharacterFormManager();
    manager->SetDefaultRule();
  }

  void TearDown() override {
    CharacterFormManager *manager =
        CharacterFormManager::GetCharacterFormManager();
    manager->SetDefaultRule();
  }
};

TEST_F(CharacterFormManagerTest, DefaultTest) {
  CharacterFormManager *manager =
      CharacterFormManager::GetCharacterFormManager();

  manager->ClearHistory();

  EXPECT_EQ(manager->GetPreeditCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("012"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("["), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("/"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("・"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("。"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("、"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("\\"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("ABC012ほげ"),
            config::Config::NO_CONVERSION);

  EXPECT_EQ(manager->GetConversionCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("012"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("["),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("/"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("・"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("。"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("、"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("\\"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("ABC012ほげ"),
            config::Config::NO_CONVERSION);

  std::string output;
  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertPreeditString("[]・。、", &output);
  EXPECT_EQ(output, "［］・。、");

  manager->ConvertPreeditString(".!@#$%^&", &output);
  EXPECT_EQ(output, "．！＠＃＄％＾＆");

  manager->ConvertPreeditString("京都東京ABCｲﾝﾀｰﾈｯﾄ012", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット０１２");

  manager->ConvertPreeditString("グーグルABCｲﾝﾀｰﾈｯﾄ012あいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネット０１２あいう");

  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("[京都]{東京}ABC!インターネット", &output);
  EXPECT_EQ(output, "［京都］｛東京｝ＡＢＣ！インターネット");

  manager->ConvertConversionString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertConversionString("[]・。、", &output);
  EXPECT_EQ(output, "［］・。、");

  manager->ConvertConversionString(".!@#$%^&", &output);
  EXPECT_EQ(output, "．！＠＃＄％＾＆");

  manager->ConvertConversionString("京都東京ABCｲﾝﾀｰﾈｯﾄ012", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット０１２");

  manager->ConvertConversionString("グーグルABCｲﾝﾀｰﾈｯﾄ012あいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネット０１２あいう");

  manager->ConvertConversionString("[京都]{東京}ABC!インターネット", &output);
  EXPECT_EQ(output, "［京都］｛東京｝ＡＢＣ！インターネット");

  // Set
  manager->SetCharacterForm("カタカナ", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("012", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("[", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("/", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("・", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("。", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("、", config::Config::HALF_WIDTH);
  manager->SetCharacterForm("\\", config::Config::HALF_WIDTH);

  // retry
  EXPECT_EQ(manager->GetPreeditCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("012"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("["), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("/"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("・"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("。"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("、"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("\\"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("ABC012ほげ"),
            config::Config::NO_CONVERSION);

  EXPECT_EQ(manager->GetConversionCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("012"),
            config::Config::HALF_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("["),
            config::Config::HALF_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("/"),
            config::Config::HALF_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("・"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("。"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("、"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("\\"),
            config::Config::HALF_WIDTH);

  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertPreeditString("[]・。、", &output);
  EXPECT_EQ(output, "［］・。、");

  manager->ConvertPreeditString(".!@#$%^&", &output);
  EXPECT_EQ(output, "．！＠＃＄％＾＆");

  manager->ConvertPreeditString("京都東京ABCｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("グーグルABCｲﾝﾀｰﾈｯﾄあいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネットあいう");

  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("[京都]{東京}ABC!インターネット", &output);
  EXPECT_EQ(output, "［京都］｛東京｝ＡＢＣ！インターネット");

  manager->ConvertConversionString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertConversionString("[]・。、", &output);
  EXPECT_EQ(output, "[]・。、");

  manager->ConvertConversionString(".!@#$%^&", &output);
  // ".!@#$%^&" will be "．！@#$%^&" by preference, but this is not
  // consistent form. so we do not convert this.
  EXPECT_EQ(output, ".!@#$%^&");

  // However we can convert separately.
  manager->ConvertConversionString(".!", &output);
  // "．！"
  EXPECT_EQ(output, "．！");
  manager->ConvertConversionString("@#$%^&", &output);
  EXPECT_EQ(output, "@#$%^&");

  manager->ConvertConversionString("京都東京ABCｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertConversionString("グーグルABCｲﾝﾀｰﾈｯﾄあいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネットあいう");

  manager->ConvertConversionString("[京都]{東京}ABC!インターネット", &output);
  // "[京都]{東京}ABC!インターネット" will be
  // "[京都]{東京}ＡＢＣ！インターネット" by preference and this is
  // not consistent
  EXPECT_EQ(output, "[京都]{東京}ABC!インターネット");

  // we can convert separately
  // "[京都]{東京}ＡＢＣ！インターネット"
  manager->ConvertConversionString("[京都]{東京}", &output);
  EXPECT_EQ(output, "[京都]{東京}");

  manager->ConvertConversionString("ABC!インターネット", &output);
  EXPECT_EQ(output, "ＡＢＣ！インターネット");

  // reset
  manager->SetCharacterForm("カタカナ", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("012", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("[", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("/", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("・", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("。", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("、", config::Config::FULL_WIDTH);
  manager->SetCharacterForm("\\", config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("012"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("["), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("/"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("・"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("。"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("、"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetPreeditCharacterForm("\\"), config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("ABC012ほげ"),
            config::Config::NO_CONVERSION);

  EXPECT_EQ(manager->GetConversionCharacterForm("カタカナ"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("012"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("["),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("/"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("・"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("。"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("、"),
            config::Config::FULL_WIDTH);

  EXPECT_EQ(manager->GetConversionCharacterForm("ABC012ほげ"),
            config::Config::NO_CONVERSION);

  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertPreeditString("[]・。、", &output);
  EXPECT_EQ(output, "［］・。、");

  manager->ConvertPreeditString(".!@#$%^&", &output);
  EXPECT_EQ(output, "．！＠＃＄％＾＆");

  manager->ConvertPreeditString("京都東京ABCｲﾝﾀｰﾈｯﾄ012", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット０１２");

  manager->ConvertPreeditString("グーグルABCｲﾝﾀｰﾈｯﾄ012あいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネット０１２あいう");

  manager->ConvertPreeditString("京都東京ABCインターネット", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット");

  manager->ConvertPreeditString("[京都]{東京}ABC!インターネット", &output);
  EXPECT_EQ(output, "［京都］｛東京｝ＡＢＣ！インターネット");

  manager->ConvertConversionString("ｲﾝﾀｰﾈｯﾄ", &output);
  EXPECT_EQ(output, "インターネット");

  manager->ConvertConversionString("[]・。、", &output);
  EXPECT_EQ(output, "［］・。、");

  manager->ConvertConversionString(".!@#$%^&", &output);
  EXPECT_EQ(output, "．！＠＃＄％＾＆");

  manager->ConvertConversionString("京都東京ABCｲﾝﾀｰﾈｯﾄ012", &output);
  EXPECT_EQ(output, "京都東京ＡＢＣインターネット０１２");

  manager->ConvertConversionString("グーグルABCｲﾝﾀｰﾈｯﾄ012あいう", &output);
  EXPECT_EQ(output, "グーグルＡＢＣインターネット０１２あいう");

  manager->ConvertConversionString("[京都]{東京}ABC!インターネット", &output);
  EXPECT_EQ(output, "［京都］｛東京｝ＡＢＣ！インターネット");
}

TEST_F(CharacterFormManagerTest, MixedFormTest) {
  CharacterFormManager *manager =
      CharacterFormManager::GetCharacterFormManager();

  manager->AddConversionRule("0", config::Config::FULL_WIDTH);
  manager->AddConversionRule(".,", config::Config::HALF_WIDTH);
  manager->AddPreeditRule("0", config::Config::FULL_WIDTH);
  manager->AddPreeditRule(".,", config::Config::HALF_WIDTH);

  std::string output;
  manager->ConvertConversionString("1.23", &output);
  EXPECT_EQ(output, "1.23");

  manager->ConvertPreeditString("1.23", &output);
  // The period is half width here
  // because require_consistent_conversion_ is false.
  EXPECT_EQ(output, "１.２３");
}

TEST_F(CharacterFormManagerTest, GroupTest) {
  CharacterFormManager *manager =
      CharacterFormManager::GetCharacterFormManager();

  {
    manager->ClearHistory();
    manager->Clear();
    manager->AddConversionRule("ア", config::Config::FULL_WIDTH);
    manager->AddPreeditRule("ア", config::Config::HALF_WIDTH);
    manager->AddConversionRule("[]", config::Config::HALF_WIDTH);
    manager->AddPreeditRule("[]", config::Config::FULL_WIDTH);
    manager->AddConversionRule("!@#$%^&*()-=", config::Config::FULL_WIDTH);
    manager->AddConversionRule("!@#$%^&*()-=", config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("["),
              config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("["),
              config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("ア"),
              config::Config::HALF_WIDTH);

    manager->SetCharacterForm("[", config::Config::FULL_WIDTH);
    manager->SetCharacterForm("ア", config::Config::FULL_WIDTH);
    manager->SetCharacterForm("@", config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("["),
              config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("["),
              config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("ア"),
              config::Config::HALF_WIDTH);
  }

  {
    manager->ClearHistory();
    manager->Clear();
    manager->AddConversionRule("ア", config::Config::FULL_WIDTH);
    manager->AddConversionRule("[]", config::Config::LAST_FORM);
    manager->AddConversionRule("!@#$%^&*()-=", config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("["),
              config::Config::FULL_WIDTH);  // default

    // same group
    manager->SetCharacterForm("]", config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("["),
              config::Config::HALF_WIDTH);
  }

  {
    manager->ClearHistory();
    manager->Clear();
    manager->AddConversionRule("ア", config::Config::FULL_WIDTH);
    manager->AddConversionRule("[](){}", config::Config::LAST_FORM);
    manager->AddConversionRule("!@#$%^&*-=", config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("{"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("}"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("("),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm(")"),
              config::Config::FULL_WIDTH);

    // same group
    manager->SetCharacterForm(")", config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("{"),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("}"),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("("),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm(")"),
              config::Config::HALF_WIDTH);
  }

  {
    manager->ClearHistory();
    manager->Clear();
    manager->AddConversionRule("ア", config::Config::FULL_WIDTH);
    manager->AddConversionRule("[](){}", config::Config::LAST_FORM);
    manager->AddPreeditRule("[](){}", config::Config::FULL_WIDTH);
    manager->AddConversionRule("!@#$%^&*-=", config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("{"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("}"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("("),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm(")"),
              config::Config::FULL_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("{"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm("}"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm("("),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm(")"),
              config::Config::FULL_WIDTH);

    // same group
    manager->SetCharacterForm(")", config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetConversionCharacterForm("{"),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("}"),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm("("),
              config::Config::HALF_WIDTH);
    EXPECT_EQ(manager->GetConversionCharacterForm(")"),
              config::Config::HALF_WIDTH);

    EXPECT_EQ(manager->GetPreeditCharacterForm("{"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm("}"),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm("("),
              config::Config::FULL_WIDTH);
    EXPECT_EQ(manager->GetPreeditCharacterForm(")"),
              config::Config::FULL_WIDTH);
  }
}

TEST_F(CharacterFormManagerTest, GetFormTypesFromStringPair) {
  CharacterFormManager::FormType f1, f2;

  EXPECT_FALSE(
      CharacterFormManager::GetFormTypesFromStringPair("", &f1, "", &f2));

  EXPECT_FALSE(
      CharacterFormManager::GetFormTypesFromStringPair("abc", &f1, "ab", &f2));

  EXPECT_FALSE(
      CharacterFormManager::GetFormTypesFromStringPair("abc", &f1, "abc", &f2));

  EXPECT_FALSE(
      CharacterFormManager::GetFormTypesFromStringPair("12", &f1, "12", &f2));

  EXPECT_FALSE(CharacterFormManager::GetFormTypesFromStringPair("あいう", &f1,
                                                                "あいう", &f2));

  EXPECT_FALSE(CharacterFormManager::GetFormTypesFromStringPair("アイウ", &f1,
                                                                "アイウ", &f2));

  EXPECT_FALSE(
      CharacterFormManager::GetFormTypesFromStringPair("愛", &f1, "恋", &f2));

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair("ABC", &f1,
                                                               "ＡＢＣ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::HALF_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::FULL_WIDTH);

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair("ａｂｃ", &f1,
                                                               "abc", &f2));
  EXPECT_EQ(f1, CharacterFormManager::FULL_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::HALF_WIDTH);

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair("おばQ", &f1,
                                                               "おばＱ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::HALF_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::FULL_WIDTH);

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair(
      "よろしくヨロシク", &f1, "よろしくﾖﾛｼｸ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::FULL_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::HALF_WIDTH);

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair(
      "よろしくグーグル", &f1, "よろしくｸﾞｰｸﾞﾙ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::FULL_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::HALF_WIDTH);

  // semi voice sound mark
  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair(
      "カッパよろしくグーグル", &f1, "ｶｯﾊﾟよろしくｸﾞｰｸﾞﾙ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::FULL_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::HALF_WIDTH);

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair(
      "ヨロシクＱ", &f1, "ﾖﾛｼｸQ", &f2));
  EXPECT_EQ(f1, CharacterFormManager::FULL_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::HALF_WIDTH);

  // mixed
  EXPECT_FALSE(CharacterFormManager::GetFormTypesFromStringPair(
      "ヨロシクQ", &f1, "ﾖﾛｼｸＱ", &f2));

  EXPECT_TRUE(CharacterFormManager::GetFormTypesFromStringPair(
      "京都Qぐーぐる", &f1, "京都Ｑぐーぐる", &f2));
  EXPECT_EQ(f1, CharacterFormManager::HALF_WIDTH);
  EXPECT_EQ(f2, CharacterFormManager::FULL_WIDTH);
}

TEST_F(CharacterFormManagerTest, InvalidStringTest) {
  CharacterFormManager *manager =
      CharacterFormManager::GetCharacterFormManager();

  std::string output;
  // "る<invalid>る"
  manager->ConvertConversionString("\xE3\x82\x8B\x88\xE3\x82\x8B", &output);

  EXPECT_EQ(output, "るる");
}

TEST_F(CharacterFormManagerTest, NumberStyle) {
  CharacterFormManager *manager =
      CharacterFormManager::GetCharacterFormManager();
  {
    std::optional<const CharacterFormManager::NumberFormStyle> stored_entry =
        manager->GetLastNumberStyle();
    EXPECT_EQ(stored_entry, std::nullopt);
  }

  CharacterFormManager::NumberFormStyle entry = {
      config::Config::FULL_WIDTH, NumberUtil::NumberString::DEFAULT_STYLE};
  manager->SetLastNumberStyle(entry);

  {
    std::optional<const CharacterFormManager::NumberFormStyle> stored_entry =
        manager->GetLastNumberStyle();
    ASSERT_NE(stored_entry, std::nullopt);
    EXPECT_EQ(stored_entry->form, entry.form);
    EXPECT_EQ(stored_entry->style, entry.style);
  }
}

}  // namespace config
}  // namespace mozc
