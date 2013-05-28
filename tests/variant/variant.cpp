#include <gtest/gtest.h>
#include <gtest/gtest-all.cc>

#include <string>

#define FL_VARIANT_WITH_TEST 1

#include <flex_lib/variant.h>

TEST(Variant, EmptyInitialization)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    Variant v;
    EXPECT_TRUE(v.IsEmpty());
}

TEST(Variant, ValueInitialization)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    {
        Variant v(10);
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(0, v.Which());
        EXPECT_EQ(10, *(int*)v.GetDataPointer());
    }
    
    {
        Variant v(20.0);
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(1, v.Which());
        EXPECT_EQ(20.0, *(double*)v.GetDataPointer());
    }
    
    {
        Variant v('0');
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(2, v.Which());
        EXPECT_EQ('0', *(char*)v.GetDataPointer());
    }
    
    {
        Variant v("abcd");
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(3, v.Which());
        EXPECT_EQ(std::string("abcd"), *(std::string*)v.GetDataPointer());
    }
    
    {
        std::string str("bcde");
        Variant v(const_cast<const std::string&>(str));
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(3, v.Which());
        EXPECT_EQ(str, *(std::string*)v.GetDataPointer());
    }
}

TEST(Variant, CopyConstruction)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    {
        Variant v1(10);
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(10, *(int*)v1.GetDataPointer());
        
        Variant v2 = v1;
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(10, *(int*)v2.GetDataPointer());
    }
    
    {
        std::string str("bcde");
        Variant v1(const_cast<const std::string&>(str));
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(3, v1.Which());
        EXPECT_EQ(str, *(std::string*)v1.GetDataPointer());
        
        Variant v2 = v1;
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(3, v2.Which());
        EXPECT_EQ(str, *(std::string*)v2.GetDataPointer());
    }
    
}

TEST(Variant, MoveConstruction)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    {
        Variant v1(10);
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(10, *(int*)v1.GetDataPointer());
        
        Variant v2(std::move(v1));
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(10, *(int*)v2.GetDataPointer());
        EXPECT_TRUE(v1.IsEmpty());
    }
    
    {
        std::string str("bcde");
        Variant v1(const_cast<const std::string&>(str));
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(3, v1.Which());
        EXPECT_EQ(str, *(std::string*)v1.GetDataPointer());
        
        Variant v2(std::move(v1));
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(3, v2.Which());
        EXPECT_EQ(str, *(std::string*)v2.GetDataPointer());
        EXPECT_TRUE(v1.IsEmpty());
    }
    
}

TEST(Variant, Destruction)
{
    int destroyCount = 0;
   
    struct TestStruct
    {
        TestStruct(int *destroyFlag) : m_destroyed(destroyFlag) {;}
        TestStruct(const TestStruct &other) : m_destroyed(other.m_destroyed)
        {
        }
        TestStruct(TestStruct &&other) : m_destroyed(other.m_destroyed)
        {
            other.m_destroyed = nullptr;
        }

        ~TestStruct() 
        {
            if (m_destroyed != nullptr)
                (*m_destroyed) ++;
        }
        
        TestStruct& operator = (const TestStruct &other)
        {
            TestStruct temp(other);
            std::swap(temp.m_destroyed, m_destroyed);
            return *this;
        }
        
        TestStruct& operator = (TestStruct &&other)
        {
            TestStruct temp(std::move(other));
            std::swap(temp.m_destroyed, m_destroyed);
            return *this;
        }
        
        int *m_destroyed;
    };
    
    typedef flex_lib::Variant<TestStruct> Variant;
    
    {
        Variant v{TestStruct(&destroyCount)};
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(0, v.Which());
        EXPECT_EQ(1, destroyCount);
    }
    EXPECT_EQ(2, destroyCount);
    
    destroyCount = 0;
    
    {
        Variant v1{TestStruct(&destroyCount)};
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(1, destroyCount);
        
        Variant v2(std::move(v1));
        EXPECT_TRUE(v1.IsEmpty());
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(1, destroyCount);
    }
    EXPECT_EQ(2, destroyCount);
    
    destroyCount = 0;
    
    {
        Variant v1{TestStruct(&destroyCount)};
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(1, destroyCount);
        
        Variant v2;
        v2 = std::move(v1);
        EXPECT_TRUE(v1.IsEmpty());
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(1, destroyCount);
    }
    EXPECT_EQ(2, destroyCount);
}

TEST(Variant, CopyAssignment)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    {
        Variant v1(10);
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(10, *(int*)v1.GetDataPointer());
        
        Variant v2;
        EXPECT_TRUE(v2.IsEmpty());
        
        v2 = v1;
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(10, *(int*)v2.GetDataPointer());
        EXPECT_FALSE(v1.IsEmpty());
    }
    
    {
        std::string str("bcde");
        Variant v1(const_cast<const std::string&>(str));
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(3, v1.Which());
        EXPECT_EQ(str, *(std::string*)v1.GetDataPointer());
        
        Variant v2;
        EXPECT_TRUE(v2.IsEmpty());
        
        v2 = v1;
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(3, v2.Which());
        EXPECT_EQ(str, *(std::string*)v2.GetDataPointer());
    }
    
}

TEST(Variant, MoveAssignment)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    {
        Variant v1(10);
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());
        EXPECT_EQ(10, *(int*)v1.GetDataPointer());
        
        Variant v2;
        EXPECT_TRUE(v2.IsEmpty());
        
        v2 = std::move(v1);
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(0, v2.Which());
        EXPECT_EQ(10, *(int*)v2.GetDataPointer());
        EXPECT_TRUE(v1.IsEmpty());
    }
    
    {
        std::string str("bcde");
        Variant v1(const_cast<const std::string&>(str));
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(3, v1.Which());
        EXPECT_EQ(str, *(std::string*)v1.GetDataPointer());
        
        Variant v2;
        EXPECT_TRUE(v2.IsEmpty());
        
        v2 = std::move(v1);
        EXPECT_FALSE(v2.IsEmpty());
        EXPECT_EQ(3, v2.Which());
        EXPECT_EQ(str, *(std::string*)v2.GetDataPointer());
        EXPECT_TRUE(v1.IsEmpty());
    }
    
}

TEST(Variant, ValueGetter)
{
    typedef flex_lib::Variant<int, double, char, std::string, int *> Variant;
    
    {
        Variant v1(10);
        EXPECT_FALSE(v1.IsEmpty());
        EXPECT_EQ(0, v1.Which());

        auto val = flex_lib::get<int>(v1);
        EXPECT_TRUE((std::is_same<decltype(val), int>::value));
        EXPECT_EQ(10, val);
        
        auto val2 = flex_lib::get<short>(v1);
        EXPECT_TRUE((std::is_same<decltype(val2), short>::value));
        EXPECT_EQ(10, val2);
        
        auto val_ptr = flex_lib::get<int>(&v1);
        EXPECT_TRUE((std::is_same<decltype(val_ptr), int*>::value));
        EXPECT_NE((int*)nullptr, val_ptr);
        EXPECT_EQ(10, *val_ptr);
        *val_ptr = 20;
        EXPECT_EQ(20, flex_lib::get<int>(v1));
        
        auto val_ptr1 = flex_lib::get<double>(&v1);
        EXPECT_EQ(nullptr, val_ptr1);
    }
    
    {
        Variant v(20.0);
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(1, v.Which());
        EXPECT_EQ(20.0, flex_lib::get<double>(v));
    }
    
    {
        Variant v('0');
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(2, v.Which());
        EXPECT_EQ('0', flex_lib::get<char>(v));
    }
    
    {
        Variant v("abcd");
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(3, v.Which());
        EXPECT_EQ(std::string("abcd"), flex_lib::get<std::string>(v));
        
        auto &str1 = flex_lib::get<const std::string&>(v);
        auto str2 = flex_lib::get<std::string>(&v);
        EXPECT_EQ(&str1, str2);
    }
    
    {
        int val = 30;
        Variant v(&val);
        EXPECT_FALSE(v.IsEmpty());
        EXPECT_EQ(4, v.Which());
        EXPECT_EQ(&val, flex_lib::get<int*>(v));
    }
}

TEST(Variant, ValueAssignment)
{
    typedef flex_lib::Variant<int, double, char, std::string> Variant;
    
    Variant v;
    v = 10;
    
    EXPECT_FALSE(v.IsEmpty());
    EXPECT_EQ(0, v.Which());
    EXPECT_EQ(10, *(int*)v.GetDataPointer());
    
    v = 20.0;
    EXPECT_FALSE(v.IsEmpty());
    EXPECT_EQ(1, v.Which());
    EXPECT_EQ(20.0, *(double*)v.GetDataPointer());

    v = '0';
    EXPECT_FALSE(v.IsEmpty());
    EXPECT_EQ(2, v.Which());
    EXPECT_EQ('0', *(char*)v.GetDataPointer());
    
    v = "abcd";
    EXPECT_FALSE(v.IsEmpty());
    EXPECT_EQ(3, v.Which());
    EXPECT_EQ(std::string("abcd"), *(std::string*)v.GetDataPointer());

    std::string str("bcde");
    v = str;
    EXPECT_FALSE(v.IsEmpty());
    EXPECT_EQ(3, v.Which());
    EXPECT_EQ(str, *(std::string*)v.GetDataPointer());
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
