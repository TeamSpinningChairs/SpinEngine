//#ifndef D3D_UTILS_H
//#define D3D_UTILS_H
//
//#include <DxErr.h>
//#pragma comment(lib, "DxErr.lib")
//// macro for directx debug
//#ifdef _DEBUG
//  #ifndef HR
//  #define HR(x)                                  \
//  {                                              \
//    HRESULT hr = x;                              \
//    if(FAILED(hr))                               \
//    {                                            \
//      ErrorIf(true, "returned: %x", hr);            \
//    }                                            \
//  }                                              
//  #endif  //HR
//#else
//  #ifndef HR
//  #define HR(x) x; // undefine HR
//  #endif
//#endif // _DEBUG
//
//#ifndef RELEASE_COM
//#define RELEASE_COM(x) \
//{                      \
//  if(x)                \
//  {                    \
//    x->Release();      \
//    x = NULL;          \
//  }                    \
//}
//#endif // RELEASE_COM
//
//// several typedefs
//typedef LPDIRECT3DDEVICE9 D3DDEVICE;
//typedef LPDIRECT3DVERTEXBUFFER9 D3DVERTEXBUFFER;
//typedef LPDIRECT3DVERTEXDECLARATION9 D3DVERTEXDECL;
//typedef LPDIRECT3DTEXTURE9 TEXTURE;
//#endif