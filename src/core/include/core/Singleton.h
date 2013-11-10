#ifndef SINGLETONPATTERN_H
#define SINGLETONPATTERN_H
namespace HighFlyers
{
	template <typename T>
	class Singleton
	{
	public:
		static T* get_instance()
		{
			if (instance == nullptr)
				instance = new T();
			return instance;
		}
	protected:
		Singleton();
		~Singleton();
	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		static T* instance;
	};
	template <typename T> T* Singleton<T>::instance = nullptr;
}

#endif // SINGLETONPATTERN_H
