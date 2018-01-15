// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/render_tokenizer.h"
#include "parser/utils.h"
#include "gtest/gtest.h"

namespace parser {



TEST(RenderTokenizerTest, TestBeginTag) {
  InputStream input;
  RenderTokenizer tokenizer;
  input.Write("<TEST t=\"a\"></TEST>");
  RenderToken token;
  bool hasNext = tokenizer.NextToken(input, token);

  EXPECT_FALSE(token.is_self_closing());
  EXPECT_EQ("test", token.tag_name());
  EXPECT_EQ("t", token.attributes()[0]->name_);
  EXPECT_EQ("a", token.attributes()[0]->value_);
  EXPECT_EQ(RenderToken::START_TAG, token.type());
  EXPECT_TRUE(token.data().empty());
  EXPECT_TRUE(hasNext);
}

TEST(RenderTokenizerTest, TestEndTag) {
  InputStream input;
  RenderTokenizer tokenizer;
  input.Write("</TEST t=\"a\">");
  RenderToken token;
  bool hasNext = tokenizer.NextToken(input, token);

  EXPECT_FALSE(token.is_self_closing());
  EXPECT_EQ("test", token.tag_name());
  EXPECT_TRUE(token.attributes().empty());
  EXPECT_EQ(RenderToken::END_TAG, token.type());
  EXPECT_TRUE(token.data().empty());
  EXPECT_TRUE(hasNext);
}

TEST(RenderTokenizerTest, TestCharTag) {
  InputStream input;
  RenderTokenizer tokenizer;
  input.Write("TEST");
  RenderToken token;
  bool hasNext = tokenizer.NextToken(input, token);

  EXPECT_FALSE(token.is_self_closing());
  EXPECT_EQ("", token.tag_name());
  EXPECT_TRUE(token.attributes().empty());
  EXPECT_EQ(RenderToken::CHARACTER, token.type());
  EXPECT_EQ("TEST", token.data());
  EXPECT_FALSE(hasNext);
}

TEST(RenderTokenizerTest, TestSelfClosingTag) {
  InputStream input;
  RenderTokenizer tokenizer;
  input.Write("<TEST t=\"a\"/>");
  RenderToken token;
  bool hasNext = tokenizer.NextToken(input, token);

  EXPECT_TRUE(token.is_self_closing());
  EXPECT_EQ("test", token.tag_name());
  EXPECT_EQ("t", token.attributes()[0]->name_);
  EXPECT_EQ("a", token.attributes()[0]->value_);
  EXPECT_EQ(RenderToken::START_TAG, token.type());
  EXPECT_TRUE(token.data().empty());
  EXPECT_TRUE(hasNext);
}

TEST(RenderTokenizerTest, ImgTag) {
  InputStream input;
  RenderTokenizer tokenizer;
  input.Write("<img t=\"a\">");
  RenderToken token;
  bool hasNext = tokenizer.NextToken(input, token);

  EXPECT_TRUE(token.is_self_closing());
  EXPECT_EQ("img", token.tag_name());
  EXPECT_EQ("t", token.attributes()[0]->name_);
  EXPECT_EQ("a", token.attributes()[0]->value_);
  EXPECT_EQ(RenderToken::START_TAG, token.type());
  EXPECT_TRUE(token.data().empty());
  EXPECT_TRUE(hasNext);
}

}  // namespace parser
