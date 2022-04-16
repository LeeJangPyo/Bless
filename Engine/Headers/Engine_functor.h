#pragma once


namespace Engine
{
	class CFinder_Tag
	{
	public:
		CFinder_Tag(const wchar_t* pTag) : m_pTag(pTag) {}
		~CFinder_Tag() = default;
	public:
		template <typename T>
		bool operator ()  (T& Pair)
		{
			return !lstrcmp(Pair.first, m_pTag);
		}
	private:
		const wchar_t* m_pTag;
	};

	class CCompare_String
	{
	public:
		CCompare_String(const wstring& wstString) : m_wstrSting(wstString) {}
		~CCompare_String() = default;
	public:
		template <typename T>
		bool operator ()  (T& Pair)
		{
			if (m_wstrSting == Pair.first)
				return true;
			return false;
		}
	private:
		wstring m_wstrSting;
	};
}