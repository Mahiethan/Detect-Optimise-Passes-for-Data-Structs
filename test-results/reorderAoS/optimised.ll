; ModuleID = 'optimised.bc'
source_filename = "unordered.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { [2 x double], double, double, double, double, [6 x i8], i8, i32, i32, float }

@arrayOne = dso_local global [1000000 x %struct.nodeOneOld] zeroinitializer, align 16
@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"\0Aa: ---\0A%d\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"\0Ab: ---\0A%f\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"\0Ac: ---\0A%d\00", align 1
@.str.4 = private unnamed_addr constant [13 x i8] c"\0Ad: ---\0A%lld\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"\0Ae: ---\0A%d\00", align 1
@.str.6 = private unnamed_addr constant [11 x i8] c"\0Ag: ---\0A%f\00", align 1
@.str.7 = private unnamed_addr constant [11 x i8] c"\0Ah: ---\0A%f\00", align 1
@.str.8 = private unnamed_addr constant [11 x i8] c"\0Ai: ---\0A%f\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNode(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i23 = alloca i32, align 4
  %j45 = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc20, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 8
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 4
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 7
  store i32 9, ptr %c, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 5
  %bf.load = load i64, ptr %d, align 8
  %bf.clear = and i64 %bf.load, -281474976710656
  %bf.set = or i64 %bf.clear, 23
  %10 = trunc i64 %bf.set to i48
  store i48 %10, ptr %d, align 8
  %11 = load ptr, ptr %array.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %12 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 6
  store i8 97, ptr %e, align 8
  store i32 0, ptr %j, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc, %for.body
  %13 = load i32, ptr %j, align 4
  %cmp10 = icmp slt i32 %13, 2
  br i1 %cmp10, label %for.body11, label %for.end

for.body11:                                       ; preds = %for.cond9
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %15 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom12
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 0
  %16 = load i32, ptr %j, align 4
  %idxprom14 = sext i32 %16 to i64
  %arrayidx15 = getelementptr inbounds [2 x double], ptr %f, i64 0, i64 %idxprom14
  store double 2.300000e+01, ptr %arrayidx15, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body11
  %17 = load i32, ptr %j, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond9, !llvm.loop !4

for.end:                                          ; preds = %for.cond9
  %18 = load ptr, ptr %array.addr, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %19 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom16
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double 2.300000e+01, ptr %g, align 8
  %20 = load ptr, ptr %array.addr, align 8
  %21 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %21 to i64
  %arrayidx19 = getelementptr inbounds %struct.nodeOneOld, ptr %20, i64 %idxprom18
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 2
  store double 0x407C852CA57A786C, ptr %h, align 8
  br label %for.inc20

for.inc20:                                        ; preds = %for.end
  %22 = load i32, ptr %i, align 4
  %inc21 = add nsw i32 %22, 1
  store i32 %inc21, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end22:                                        ; preds = %for.cond
  store i32 0, ptr %i23, align 4
  br label %for.cond24

for.cond24:                                       ; preds = %for.inc63, %for.end22
  %23 = load i32, ptr %i23, align 4
  %24 = load i32, ptr %size.addr, align 4
  %cmp25 = icmp slt i32 %23, %24
  br i1 %cmp25, label %for.body26, label %for.end65

for.body26:                                       ; preds = %for.cond24
  %25 = load i32, ptr %i23, align 4
  %idxprom27 = sext i32 %25 to i64
  %arrayidx28 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom27
  %a29 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx28, i32 0, i32 8
  store i32 1, ptr %a29, align 8
  %26 = load i32, ptr %i23, align 4
  %idxprom30 = sext i32 %26 to i64
  %arrayidx31 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom30
  %b32 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx31, i32 0, i32 4
  store double 1.000000e+01, ptr %b32, align 8
  %27 = load i32, ptr %i23, align 4
  %idxprom33 = sext i32 %27 to i64
  %arrayidx34 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom33
  %c35 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx34, i32 0, i32 7
  store i32 9, ptr %c35, align 8
  %28 = load i32, ptr %i23, align 4
  %idxprom36 = sext i32 %28 to i64
  %arrayidx37 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom36
  %d38 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx37, i32 0, i32 5
  %bf.load39 = load i64, ptr %d38, align 8
  %bf.clear40 = and i64 %bf.load39, -281474976710656
  %bf.set41 = or i64 %bf.clear40, 23
  %29 = trunc i64 %bf.set41 to i48
  store i48 %29, ptr %d38, align 8
  %30 = load i32, ptr %i23, align 4
  %idxprom42 = sext i32 %30 to i64
  %arrayidx43 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom42
  %e44 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx43, i32 0, i32 6
  store i8 97, ptr %e44, align 8
  store i32 0, ptr %j45, align 4
  br label %for.cond46

for.cond46:                                       ; preds = %for.inc54, %for.body26
  %31 = load i32, ptr %j45, align 4
  %cmp47 = icmp slt i32 %31, 2
  br i1 %cmp47, label %for.body48, label %for.end56

for.body48:                                       ; preds = %for.cond46
  %32 = load ptr, ptr %array.addr, align 8
  %33 = load i32, ptr %i23, align 4
  %idxprom49 = sext i32 %33 to i64
  %arrayidx50 = getelementptr inbounds %struct.nodeOneOld, ptr %32, i64 %idxprom49
  %f51 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx50, i32 0, i32 0
  %34 = load i32, ptr %j45, align 4
  %idxprom52 = sext i32 %34 to i64
  %arrayidx53 = getelementptr inbounds [2 x double], ptr %f51, i64 0, i64 %idxprom52
  store double 2.300000e+01, ptr %arrayidx53, align 8
  br label %for.inc54

for.inc54:                                        ; preds = %for.body48
  %35 = load i32, ptr %j45, align 4
  %inc55 = add nsw i32 %35, 1
  store i32 %inc55, ptr %j45, align 4
  br label %for.cond46, !llvm.loop !7

for.end56:                                        ; preds = %for.cond46
  %36 = load i32, ptr %i23, align 4
  %idxprom57 = sext i32 %36 to i64
  %arrayidx58 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom57
  %g59 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx58, i32 0, i32 1
  store double 2.300000e+01, ptr %g59, align 8
  %37 = load i32, ptr %i23, align 4
  %idxprom60 = sext i32 %37 to i64
  %arrayidx61 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom60
  %h62 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx61, i32 0, i32 2
  store double 0x407C852CA57A786C, ptr %h62, align 8
  br label %for.inc63

for.inc63:                                        ; preds = %for.end56
  %38 = load i32, ptr %i23, align 4
  %inc64 = add nsw i32 %38, 1
  store i32 %inc64, ptr %i23, align 4
  br label %for.cond24, !llvm.loop !8

for.end65:                                        ; preds = %for.cond24
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @calculationsOne(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  %j14 = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc32, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end34

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc29, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end31

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 8
  %4 = load i32, ptr %a, align 8
  %add = add nsw i32 %4, 1
  store i32 %add, ptr %a, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom4
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 4
  %6 = load double, ptr %b, align 8
  %add6 = fadd double %6, 1.000000e-03
  store double %add6, ptr %b, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %7 to i64
  %arrayidx8 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom7
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 7
  %8 = load i32, ptr %c, align 8
  %add9 = add nsw i32 %8, 1
  store i32 %add9, ptr %c, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom10
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 5
  %bf.load = load i64, ptr %d, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %add12 = add nsw i64 %bf.ashr, 1
  %bf.load13 = load i64, ptr %d, align 8
  %bf.value = and i64 %add12, 281474976710655
  %bf.clear = and i64 %bf.load13, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %10 = trunc i64 %bf.set to i48
  store i48 %10, ptr %d, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  store i32 0, ptr %j14, align 4
  br label %for.cond15

for.cond15:                                       ; preds = %for.inc, %for.body3
  %11 = load i32, ptr %j14, align 4
  %cmp16 = icmp slt i32 %11, 2
  br i1 %cmp16, label %for.body17, label %for.end

for.body17:                                       ; preds = %for.cond15
  %12 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %12 to i64
  %arrayidx19 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom18
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 0
  %13 = load i32, ptr %j14, align 4
  %idxprom20 = sext i32 %13 to i64
  %arrayidx21 = getelementptr inbounds [2 x double], ptr %f, i64 0, i64 %idxprom20
  %14 = load double, ptr %arrayidx21, align 8
  %add22 = fadd double %14, 1.000000e-03
  store double %add22, ptr %arrayidx21, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body17
  %15 = load i32, ptr %j14, align 4
  %inc = add nsw i32 %15, 1
  store i32 %inc, ptr %j14, align 4
  br label %for.cond15, !llvm.loop !9

for.end:                                          ; preds = %for.cond15
  %16 = load i32, ptr %i, align 4
  %idxprom23 = sext i32 %16 to i64
  %arrayidx24 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom23
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx24, i32 0, i32 1
  %17 = load double, ptr %g, align 8
  %add25 = fadd double %17, 1.000000e-03
  store double %add25, ptr %g, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom26 = sext i32 %18 to i64
  %arrayidx27 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom26
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx27, i32 0, i32 2
  %19 = load double, ptr %h, align 8
  %add28 = fadd double %19, 1.000000e-03
  store double %add28, ptr %h, align 8
  br label %for.inc29

for.inc29:                                        ; preds = %for.end
  %20 = load i32, ptr %i, align 4
  %inc30 = add nsw i32 %20, 1
  store i32 %inc30, ptr %i, align 4
  br label %for.cond1, !llvm.loop !10

for.end31:                                        ; preds = %for.cond1
  br label %for.inc32

for.inc32:                                        ; preds = %for.end31
  %21 = load i32, ptr %j, align 4
  %inc33 = add nsw i32 %21, 1
  store i32 %inc33, ptr %j, align 4
  br label %for.cond, !llvm.loop !11

for.end34:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @calculationsTwo(i32 noundef %size, ptr noundef %array) #0 {
entry:
  %size.addr = alloca i32, align 4
  %array.addr = alloca ptr, align 8
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  %j29 = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc58, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end60

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc55, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end57

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 8
  %4 = load i32, ptr %a, align 8
  %5 = load ptr, ptr %array.addr, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %6 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %5, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 8
  %7 = load i32, ptr %a6, align 8
  %add = add nsw i32 %7, %4
  store i32 %add, ptr %a6, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %8 to i64
  %arrayidx8 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom7
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 4
  %9 = load double, ptr %b, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %11 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom9
  %b11 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 4
  %12 = load double, ptr %b11, align 8
  %add12 = fadd double %12, %9
  store double %add12, ptr %b11, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %13 to i64
  %arrayidx14 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 7
  %14 = load i32, ptr %c, align 8
  %15 = load ptr, ptr %array.addr, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %16 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom15
  %c17 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx16, i32 0, i32 7
  %17 = load i32, ptr %c17, align 8
  %add18 = add nsw i32 %17, %14
  store i32 %add18, ptr %c17, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom19 = sext i32 %18 to i64
  %arrayidx20 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom19
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx20, i32 0, i32 5
  %bf.load = load i64, ptr %d, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %19 = load ptr, ptr %array.addr, align 8
  %20 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %20 to i64
  %arrayidx22 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i64 %idxprom21
  %d23 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 5
  %bf.load24 = load i64, ptr %d23, align 8
  %bf.shl25 = shl i64 %bf.load24, 16
  %bf.ashr26 = ashr i64 %bf.shl25, 16
  %add27 = add nsw i64 %bf.ashr26, %bf.ashr
  %bf.load28 = load i64, ptr %d23, align 8
  %bf.value = and i64 %add27, 281474976710655
  %bf.clear = and i64 %bf.load28, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %21 = trunc i64 %bf.set to i48
  store i48 %21, ptr %d23, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  store i32 0, ptr %j29, align 4
  br label %for.cond30

for.cond30:                                       ; preds = %for.inc, %for.body3
  %22 = load i32, ptr %j29, align 4
  %cmp31 = icmp slt i32 %22, 2
  br i1 %cmp31, label %for.body32, label %for.end

for.body32:                                       ; preds = %for.cond30
  %23 = load i32, ptr %i, align 4
  %idxprom33 = sext i32 %23 to i64
  %arrayidx34 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom33
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx34, i32 0, i32 0
  %24 = load i32, ptr %j29, align 4
  %idxprom35 = sext i32 %24 to i64
  %arrayidx36 = getelementptr inbounds [2 x double], ptr %f, i64 0, i64 %idxprom35
  %25 = load double, ptr %arrayidx36, align 8
  %26 = load ptr, ptr %array.addr, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom37 = sext i32 %27 to i64
  %arrayidx38 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom37
  %f39 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx38, i32 0, i32 0
  %28 = load i32, ptr %j29, align 4
  %idxprom40 = sext i32 %28 to i64
  %arrayidx41 = getelementptr inbounds [2 x double], ptr %f39, i64 0, i64 %idxprom40
  %29 = load double, ptr %arrayidx41, align 8
  %add42 = fadd double %29, %25
  store double %add42, ptr %arrayidx41, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body32
  %30 = load i32, ptr %j29, align 4
  %inc = add nsw i32 %30, 1
  store i32 %inc, ptr %j29, align 4
  br label %for.cond30, !llvm.loop !12

for.end:                                          ; preds = %for.cond30
  %31 = load i32, ptr %i, align 4
  %idxprom43 = sext i32 %31 to i64
  %arrayidx44 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom43
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx44, i32 0, i32 1
  %32 = load double, ptr %g, align 8
  %33 = load ptr, ptr %array.addr, align 8
  %34 = load i32, ptr %i, align 4
  %idxprom45 = sext i32 %34 to i64
  %arrayidx46 = getelementptr inbounds %struct.nodeOneOld, ptr %33, i64 %idxprom45
  %g47 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx46, i32 0, i32 1
  %35 = load double, ptr %g47, align 8
  %add48 = fadd double %35, %32
  store double %add48, ptr %g47, align 8
  %36 = load i32, ptr %i, align 4
  %idxprom49 = sext i32 %36 to i64
  %arrayidx50 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom49
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx50, i32 0, i32 2
  %37 = load double, ptr %h, align 8
  %38 = load ptr, ptr %array.addr, align 8
  %39 = load i32, ptr %i, align 4
  %idxprom51 = sext i32 %39 to i64
  %arrayidx52 = getelementptr inbounds %struct.nodeOneOld, ptr %38, i64 %idxprom51
  %h53 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx52, i32 0, i32 2
  %40 = load double, ptr %h53, align 8
  %add54 = fadd double %40, %37
  store double %add54, ptr %h53, align 8
  br label %for.inc55

for.inc55:                                        ; preds = %for.end
  %41 = load i32, ptr %i, align 4
  %inc56 = add nsw i32 %41, 1
  store i32 %inc56, ptr %i, align 4
  br label %for.cond1, !llvm.loop !13

for.end57:                                        ; preds = %for.cond1
  br label %for.inc58

for.inc58:                                        ; preds = %for.end57
  %42 = load i32, ptr %j, align 4
  %inc59 = add nsw i32 %42, 1
  store i32 %inc59, ptr %j, align 4
  br label %for.cond, !llvm.loop !14

for.end60:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %n = alloca i32, align 4
  %arrayTwoOld = alloca ptr, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  %0 = load ptr, ptr %argv.addr, align 8
  %arrayidx = getelementptr inbounds ptr, ptr %0, i64 1
  %1 = load ptr, ptr %arrayidx, align 8
  %call = call i32 @atoi(ptr noundef %1) #5
  store i32 %call, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %conv = sext i32 %2 to i64
  %mul = mul i64 %conv, 88
  %3 = udiv i64 %mul, 88
  %4 = mul i64 %3, 72
  %5 = call ptr @malloc(i64 %4)
  store ptr %5, ptr %arrayTwoOld, align 8
  %6 = load ptr, ptr %arrayTwoOld, align 8
  %7 = load i32, ptr %n, align 4
  call void @populateNode(ptr noundef %6, i32 noundef %7)
  %8 = load i32, ptr %n, align 4
  call void @calculationsOne(i32 noundef %8)
  %9 = load i32, ptr %n, align 4
  %10 = load ptr, ptr %arrayTwoOld, align 8
  call void @calculationsTwo(i32 noundef %9, ptr noundef %10)
  %call2 = call i32 @rand() #6
  %11 = load i32, ptr %n, align 4
  %rem = srem i32 %call2, %11
  store i32 %rem, ptr %i, align 4
  %call3 = call i32 @rand() #6
  %rem4 = srem i32 %call3, 67
  store i32 %rem4, ptr %j, align 4
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %12 = load ptr, ptr %arrayTwoOld, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom = sext i32 %13 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 8
  %14 = load i32, ptr %a, align 8
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %14)
  %15 = load ptr, ptr %arrayTwoOld, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %16 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom8
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx9, i32 0, i32 4
  %17 = load double, ptr %b, align 8
  %call10 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %17)
  %18 = load ptr, ptr %arrayTwoOld, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %19 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom11
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 7
  %20 = load i32, ptr %c, align 8
  %call13 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %20)
  %21 = load ptr, ptr %arrayTwoOld, align 8
  %22 = load i32, ptr %i, align 4
  %idxprom14 = sext i32 %22 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeOneOld, ptr %21, i64 %idxprom14
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx15, i32 0, i32 5
  %bf.load = load i64, ptr %d, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %call16 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i64 noundef %bf.ashr)
  %23 = load ptr, ptr %arrayTwoOld, align 8
  %24 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %24 to i64
  %arrayidx18 = getelementptr inbounds %struct.nodeOneOld, ptr %23, i64 %idxprom17
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx18, i32 0, i32 6
  %25 = load i8, ptr %e, align 8
  %conv19 = sext i8 %25 to i32
  %call20 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %conv19)
  %26 = load ptr, ptr %arrayTwoOld, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %27 to i64
  %arrayidx22 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom21
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 1
  %28 = load double, ptr %g, align 8
  %call23 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, double noundef %28)
  %29 = load ptr, ptr %arrayTwoOld, align 8
  %30 = load i32, ptr %i, align 4
  %idxprom24 = sext i32 %30 to i64
  %arrayidx25 = getelementptr inbounds %struct.nodeOneOld, ptr %29, i64 %idxprom24
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx25, i32 0, i32 2
  %31 = load double, ptr %h, align 8
  %call26 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, double noundef %31)
  %32 = load ptr, ptr %arrayTwoOld, align 8
  %33 = load i32, ptr %i, align 4
  %idxprom27 = sext i32 %33 to i64
  %arrayidx28 = getelementptr inbounds %struct.nodeOneOld, ptr %32, i64 %idxprom27
  %i29 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx28, i32 0, i32 3
  %34 = load double, ptr %i29, align 8
  %call30 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, double noundef %34)
  ret i32 0
}

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef) #1

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #2

; Function Attrs: nounwind
declare dso_local i32 @rand() #3

declare dso_local i32 @printf(ptr noundef, ...) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind willreturn memory(read) }
attributes #6 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
!11 = distinct !{!11, !5}
!12 = distinct !{!12, !5}
!13 = distinct !{!13, !5}
!14 = distinct !{!14, !5}
