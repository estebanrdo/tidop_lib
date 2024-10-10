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
 
#define BOOST_TEST_MODULE Tidop messages test
#include <boost/test/unit_test.hpp>
#include <tidop/core/messages.h>
#include "tidop/core/app.h"
#include "tidop/core/console.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace tl;


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



//class MessageBuffer
//  : public std::streambuf
//{
//
//private:
//
//  std::vector<std::streambuf *> _sb;
//  //std::streambuf *sb2_;
//
//  MessageBuffer()
//    : _sb(0)
//  {
//  }
//
//public:
//
//  static MessageBuffer &instance()
//  {
//    static MessageBuffer message_buffer;
//    return message_buffer;
//  }
//
//  void subscribe(std::streambuf *sb)
//  {
//    _sb.push_back(sb);
//  }
//
//  int overflow(int c)
//  {
//    typedef std::streambuf::traits_type traits;
//    //bool rc(true);
//    std::vector<bool> rc(_sb.size(), true);
//    if (!traits::eq_int_type(traits::eof(), c)) {
//      
//      for (size_t i = 0; i < _sb.size(); i++) {
//          traits::eq_int_type(this->_sb[i]->sputc(c), traits::eof()) && (rc[i] = false);
//      }
//
//    }
//
//    bool r = false;
//    for (auto c : rc) {
//      if (c) {
//        r = true;
//        break;
//      }
//    }
//
//    return r ? traits::not_eof(c) : traits::eof();
//  }
//
//  int sync()
//  {
//    bool rc(true);
//    for (size_t i = 0; i < _sb.size(); i++)
//      this->_sb[i]->pubsync() != -1 || (rc = false);
//    return rc ? 0 : -1;
//  }
//};


class MessageHandler
  : public std::ostream
{

public:
  
  MessageHandler(std::streambuf *buff);
  ~MessageHandler();

  MessageHandler &operator <<(Level level)
  {
    Console2::instance() << level;

    return *this;
  }

  MessageHandler &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);/*
	{
		*this << _endl;
	  App::instance().console().reset();
		return *this;
	}*/

private:

};

  inline MessageHandler &MessageHandler::operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
	{
		*this << _endl;
	  Console2::instance().reset();
		return *this;
	}

MessageHandler::MessageHandler(std::streambuf *buff)
  : std::ostream(buff)
{
}

MessageHandler::~MessageHandler()
{
}



BOOST_FIXTURE_TEST_CASE(default_constructor, MessageTest)
{
  //Message message;

  //std::ostream &kk = std::cout;
  //kk  << "sdfsdf";
  //message << Message::Level::debug << "Hello World" << std::endl;

  std::ofstream fout("D:\\Desarrollo\\libs_sources\\tidoplib\\test\\experimental\\message\\tee.txt", std::ofstream::app);
  //teebuf        sbuf(fout.rdbuf(), std::cout.rdbuf());
  //std::ostream  out(&sbuf);
  //out << "hello, world!\n";

  Console2 &console = Console2::instance();
  console << Level::error << "Mensaje de error por consola" << std::endl;
  console << Level::warning << "Warning por consola" << std::endl;
  console << Level::info << "Información por consola" << std::endl;
  console << "Impresión por consola" << std::endl;

  Console2::debug() << "Mensaje de depuración por consola" << std::endl;
  Console2::info() << "Mensaje de información por consola" << std::endl;
  Console2::warning() << "Mensaje de warning por consola" << std::endl;
  Console2::error() << "Mensaje de error por consola" << std::endl;

  MessageBuffer msgbuff = MessageBuffer::instance();
  msgbuff.subscribe(fout.rdbuf());
  //msgbuff.subscribe(std::cout.rdbuf());
  msgbuff.subscribe(console.rdbuf());
  //std::ostream  out(&msgbuff);
  //out << "hello, world!\n";
  
  //console.stream() << "Console" << std::endl;
  fout << "Log" << std::endl;

  MessageHandler message_manager(&msgbuff);
  message_manager << Level::info << "MessageManager" << std::endl;


}


BOOST_AUTO_TEST_SUITE_END()
