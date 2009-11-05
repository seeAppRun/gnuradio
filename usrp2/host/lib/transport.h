/* -*- c++ -*- */
/*
 * Copyright 2009 Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_TRANSPORT_H
#define INCLUDED_TRANSPORT_H

#include <boost/thread.hpp>
#include <cstring>
#include <sys/uio.h>
#include "sbuff.h"

namespace usrp2 {

  class transport {
  public:
    typedef boost::function<void(sbuff::sptr)> callback_t;
    typedef boost::shared_ptr<transport> sptr;
  private:
    std::string              d_type_str;
    volatile bool            d_running;
    boost::thread            *d_thread;
    callback_t               d_cb;
    void run();
  public:
    /*!
     * \brief create a new transport
     * The callback takes a void * pointer and a length in bytes.
     * \param type_str a descriptive string
     */
    transport(const std::string &type_str);
    virtual ~transport();
    /*!
     * \brief Set the callback
     * \param cb the callback created by boost::bind
     */
    void set_callback(callback_t cb){d_cb=cb;}
    /*!
     * \brief create a new thread for receiving
     */
    void start();
    /*!
     * \brief stop and join the current thread
     */
    void stop();
    /*!
     * \brief called from thread on init (override in a subclass)
     * Purpose: to have a thread initialization hook.
     */
    virtual void init();
    /*!
     * \brief send the contents of the buffer (override in a subclass)
     * \param iovec a list of iovecs
     * \param iovlen the number of iovecs
     * \return the number of bytes sent, -1 for error
     */
    virtual int sendv(const iovec *iov, size_t iovlen);
    /*!
     * \brief receive data into the sbuffer (override in a subclass)
     * \return a new sbuff, for now, an empty sbuff means nothing was recvd
     */
    virtual sbuff::sptr recv();
  };
  
} // namespace usrp2

#endif /* INCLUDED_TRANSPORT_H */
