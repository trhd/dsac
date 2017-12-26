#include <assert.h>
#include "mergesort.h"

static void
ptrcpy(void const ** to, void const ** from, unsigned int count)
{
	while (count --> 0)
		*to++ = *from++;
}

void
mergesort(void const ** data, unsigned int size, int (*cmp) (void const *, void const *))
{
	assert(data);
	assert(size);
	assert(cmp);

	unsigned int const split = size / 2;
	void const * tmp[split];

	if (size == 1)
		return;

	if (size > 2)
	{
		mergesort(data, split, cmp);
		mergesort(data + split, size - split, cmp);
	}

	ptrcpy(tmp, data, split);

	for (unsigned int i = 0, j = split, k = 0; i < split ; )
	{
		if (j == size)
		{
			while (i < split)
				data[k++] = tmp[i++];
			break;
		}

		if (cmp(tmp[i], data[j]) < 0)
				data[k++] = tmp[i++];
		else
			data[k++] = data[j++];
	}
}
