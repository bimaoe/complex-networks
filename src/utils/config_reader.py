"""This file generates networks with parameters specified in parameter_list_dict."""
import argparse

def convert(vartype, x):
  if vartype == 'int':
    return int(x)
  elif vartype == 'float':
    return float(x)
  elif vartype == 'bool':
    return x in ['True', 'true']

def parse(config_filename):
  config_file =open(config_filename, 'r')
  config = {}
  stack = []

  curr = config
  next = None
  for line in config_file:
    if not line:
      continue
    var = line.strip().split(' ')
    if var[0] == '#':
      continue
    elif var[0] == '+':
      stack.append(curr)
      curr = next
    elif var[0] == '-':
      curr = stack.pop()

    if isinstance(curr, dict):
      print 'eh dict'
      if var[0] == 'single':
        curr[var[1]] = var[2]
      elif var[0] == 'list':
        if len(var) > 2:
          curr[var[1]] = var[2].split('|')
        else:
          curr[var[1]] = []
        next = curr[var[1]]
      elif var[0] == 'dict':
        curr[var[1]] = {}
        next = curr[var[1]]
      elif var[0] == 'tsingle':
        curr[var[2]] = convert(var[1], var[3])
      elif var[0] == 'tlist':
        curr[var[2]] = [convert(var[1], _x) for _x in var[3].split('|')]
        next = curr[var[2]]
    elif isinstance(curr, list):
      print 'eh list'
      if var[0] == 'single':
        curr.append(var[1])
      elif var[0] == 'list':
        if len(var) > 1:
          curr.append(var[1].split('|'))
        else:
          curr.append([])
        next = curr[-1]
      elif var[0] == 'dict':
        curr.append({})
        next = curr[-1]
      elif var[0] == 'tsingle':
        curr.append(convert(var[1], var[2]))
      elif var[0] == 'tlist':
        curr.append([convert(var[1], _x) for _x in var[2].split('|')])
        next = curr[-1]
  return config

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-c', '--config_filename',
      help='File where the parameters are.', required=True)
  
  args = vars(parser.parse_args())
  config_filename = args['config_filename']
  
  config = parse(config_filename)
  print config

if __name__ == '__main__':
  main()
  