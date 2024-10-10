/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop message test
#include <boost/test/unit_test.hpp>
#include <tidop/experimental/message.h>

using namespace tl;
using namespace message;

BOOST_AUTO_TEST_SUITE(MessageTestSuite)


struct MessageTest
{
  MessageTest()
  {
  }

  ~MessageTest()
  {
  }

  void setup()
  {

  }

  void teardown()
  {

  }

};

struct teebuf
  : std::streambuf
{
  std::streambuf *sb1_;
  std::streambuf *sb2_;

  teebuf(std::streambuf *sb1, std::streambuf *sb2)
    : sb1_(sb1), sb2_(sb2)
  {
  }
  int overflow(int c)
  {
    typedef std::streambuf::traits_type traits;
    bool rc(true);
    if (!traits::eq_int_type(traits::eof(), c)) {
      traits::eq_int_type(this->sb1_->sputc(c), traits::eof())
        && (rc = false);
      traits::eq_int_type(this->sb2_->sputc(c), traits::eof())
        && (rc = false);
    }
    return rc ? traits::not_eof(c) : traits::eof();
  }
  int sync()
  {
    bool rc(true);
    this->sb1_->pubsync() != -1 || (rc = false);
    this->sb2_->pubsync() != -1 || (rc = false);
    return rc ? 0 : -1;
  }
};

#include <fstream>
#include <iostream>


BOOST_FIXTURE_TEST_CASE(default_constructor, MessageTest)
{
  //Message message;

  //std::ostream &kk = std::cout;
  //kk  << "sdfsdf";
  //message << Message::Level::debug << "" << std::endl;

  std::ofstream fout("tee.txt");
  teebuf        sbuf(fout.rdbuf(), std::cout.rdbuf());
  std::ostream  out(&sbuf);
  out << "hello, world!\n";

}


BOOST_AUTO_TEST_SUITE_END()