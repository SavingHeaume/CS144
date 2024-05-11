#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return is_closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if ( is_closed_ ) {
    return;
  }

  if ( data.size() > available_capacity() ) {
    data.resize( available_capacity() );
  }

  if ( !data.empty() ) {
    pushed_num_ += data.size();
    buffered_num_ += data.size();
    bytes_.emplace( move( data ) );
  }

  if ( next_view_.empty() && !bytes_.empty() ) {
    next_view_ = bytes_.front();
  }
}

void Writer::close()
{
  // Your code here.
  if ( !is_closed_ ) {
    is_closed_ = true;
    bytes_.emplace( string( 1, EOF ) );
  }
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buffered_num_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return pushed_num_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return is_closed_ && buffered_num_ == 0;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return popped_num_;
}

string_view Reader::peek() const
{
  // Your code here.
  // return string_view( bytes_.front() );
  return next_view_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.

  /*
  if ( bytes_.empty() ) {
    return;
  }

  if ( bytes_.size() < len ) {
    len = bytes_.size();
  }

  for ( uint64_t i = 0; i < len; i++ ) {
    bytes_.pop();
  }
  */

  auto remainder = len;
  while ( remainder >= next_view_.size() && remainder != 0 ) {
    // 不断清掉能从队列中 pop 出去的字节
    remainder -= next_view_.size();
    bytes_.pop();
    next_view_ = bytes_.empty() ? ""sv : bytes_.front();
  }
  if ( !next_view_.empty() )
    next_view_.remove_prefix( remainder );

  buffered_num_ -= len;
  popped_num_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffered_num_;
}
