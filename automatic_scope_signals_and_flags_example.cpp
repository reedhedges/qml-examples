#define DEFINE_SCOPE_FLAG(name) \
        private: bool name##_flag = false; \
    	template <class C> struct name { \
        	C* obj; \
            name(C* _obj) : obj(_obj) { obj->name##_flag = true; } \
            ~name() { obj->name##_flag = false; } \
        };

#define DEFINE_SCOPE_FLAG_WITH_SIGNAL(name) \
        private: bool name##_flag = false; \
        signals: void name##Changed(bool value); \
        private: \
            void _set_##name##_flag(bool val) { name##_flag = val; emit name##Changed(name##_flag); } \
            template <class C> struct name { \
                C* obj; \
                name(C* _obj) : obj(_obj) { obj->_set_##name##_flag(true); } \
                ~name() { obj->_set_##name##_flag(false); } \
            };

#define DEFINE_SCOPE_SIGNALS(name) \
        signals: \
            void name##Started(); \
            void name##Stopped(); \
        private: \
            void _signal_##name##_started() { emit name##Started(); } \
            void _signal_##name##_stopped() { emit name##Stopped(); } \
            template <class C> struct name { \
                C* obj; \
                name(C* _obj) : obj(_obj) { obj->_signal_##name##_started(); } \
                ~name() { obj->_signal_##name##_stopped(); } \
            };

#define USE_SCOPE_FLAG(name) name scope_flag_##name(this); 

struct Foo {
  private:

  DEFINE_SCOPE_FLAG(MyFlag1)
  DEFINE_SCOPE_FLAG_WITH_SIGNAL(MyFlag2)
  DEFINE_SCOPE_SIGNALS(Test3)
    
  void test() {
    MyFlag1 f1(this);
    MyFlag2 f2(this);
  }
  
  void test2() {
    USE_SCOPE_FLAG(MyFlag1);
  }
  
  void test3() {
    Test3 t(this);
  }
};

int main()
{
    
    test();
    test2();
    test3();
    return 0;
}

