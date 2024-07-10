from typing import List

template = '''
template <{template_args}>
struct {name}<{template_spec}> {{
	template <auto func>
	static decltype(auto) {callconv} wrap({wrap_args}) {{
		{wrap_body}
	}}
	template <auto func>
	static decltype(auto) invoke({invoke_args}) {{
		{invoke_body}
	}}
}};
'''

LETTERS = 'ABCD'

def gen(args: List[bool], is_membercall: bool):
	template_args = ['R']

	# n is the index for the last float arg
	n = max(i if v else -1 for i, v in enumerate(args))
	for i, v in enumerate(args):
		if not v and i < n:
			template_args.append(LETTERS[i])
	
	callconv = '__thiscall' if is_membercall else '__fastcall'

	template_args.append('...Args')
	template_spec = 'R(' + ', '.join(tuple('float' if v else LETTERS[i] for i, v in enumerate(args) if i <= n) + ('Args...',)) + ')'

	wrap_args = ', '.join(tuple(f'{LETTERS[i]} a{i}' for i, v in enumerate(args) if not v and i <= n) + ('Args... args',))
	wrap_body = ''
	if any(args):
		wrap_body = 'float ' + ', '.join(f'a{i}' for i, v in enumerate(args) if v) + ';\n\t\t'
		wrap_body += '\n\t\t'.join(f'__asm movss a{i}, xmm{i}' for i, v in enumerate(args) if v) + '\n\t\t'
	wrap_body += 'return func(' + ', '.join(tuple(f'a{i}' for i in range(len(args)) if i <= n) + ('args...',)) + ');'

	invoke_args = ', '.join(tuple(f'float a{i}' if v else f'{LETTERS[i]} a{i}' for i, v in enumerate(args) if i <= n) + ('Args... args',))
	invoke_body = ''
	invoke_body += '\n\t\t'.join(f'__asm movss xmm{i}, a{i}' for i, v in enumerate(args) if v) + '\n\t\t'
	invoke_type_args = ', '.join(tuple(LETTERS[i] for i, v in enumerate(args) if not v and i <= n) + ('Args...',))
	invoke_call_args = ', '.join(tuple(f'a{i}' for i, v in enumerate(args) if not v and i <= n) + ('args...',))
	invoke_body += f'return reinterpret_cast<R({callconv}*)({invoke_type_args})>(tramp<func>)({invoke_call_args});'

	return template.format(
		name = 'membercall' if is_membercall else 'optcall',
		template_args = ', '.join(f'class {i}' for i in template_args),
		template_spec = template_spec,
		callconv = callconv,
		wrap_args = wrap_args,
		wrap_body = wrap_body,
		invoke_args = invoke_args,
		invoke_body = invoke_body
	)

# print(gen([False, True, False, False], is_membercall=False))

with open('gen.hpp', 'w') as file:
	# this is a very silly way of going through all the possible combinations
	for i in range(2**5):
		is_membercall, *args = [j == '1' for j in f'{i:05b}']
		if is_membercall and args[0]: continue
		file.write(gen(args, is_membercall))
		file.write('\n')
