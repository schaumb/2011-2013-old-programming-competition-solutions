#define OWNERSHIP
bool HasSign( OWNERSHIP char const* string, char const sign)
{
  bool Found = false;
  while( char Act = *string )
  {
    if ( Act == sign )
    {
      Found = true;
      break;
    }
    ++string;
  }
  delete [] string;
  return Found;
}

int main() {

}

